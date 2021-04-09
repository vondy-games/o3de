"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""

import sys
import os
import azlmbr.asset as asset
import azlmbr.atom
import azlmbr.bus
import azlmbr.math as math
import azlmbr.name
import azlmbr.paths
import azlmbr.shadermanagementconsole
import azlmbr.shader
import collections
from PySide2 import QtWidgets

PROJECT_SHADER_VARIANTS_FOLDER = "ShaderVariants"

def prompt_message_box(text, informativeText = None, qButtons = QtWidgets.QMessageBox.Yes|QtWidgets.QMessageBox.No|QtWidgets.QMessageBox.Cancel):
    msgBox =  QtWidgets.QMessageBox()
    msgBox.setText(text)
    if informativeText:
        msgBox.setInformativeText(informativeText)
    msgBox.setStandardButtons(qButtons)
    
    return msgBox.exec()

def clean_existing_shadervariantlist_files(filePaths):
    for file in filePaths:
        if os.path.exists(file):
            os.remove(file)

def main():
    print("==== Begin shader variant script ==========================================================")

    if len(sys.argv) != 2:
        print("The script requires a .shader file as input argument")
        return

    filename = sys.argv[1]
    suffix, extension = filename.split(".", 1)

    if extension != "shader":
        print("The input argument for the script is not a valid .shader file")
        return
    
    # Get info such as relative path of the file and asset id
    shaderAssetInfo = azlmbr.shadermanagementconsole.ShaderManagementConsoleRequestBus(
        azlmbr.bus.Broadcast, 
        'GetSourceAssetInfo', 
        filename
    )
    
    # retrieves a list of all material source files that use the shader. Note that materials inherit from materialtype files, which are actual files that refer to shader files.
    materialAssetIds = azlmbr.shadermanagementconsole.ShaderManagementConsoleRequestBus(
        azlmbr.bus.Broadcast, 
        'FindMaterialAssetsUsingShader', 
        shaderAssetInfo.relativePath
    )
    
    response = prompt_message_box(
        "Generating .shadervariantlist File",
        "This process may take a while. Would you like to save the generated .shadervariantlist file in the project folder? " \
            "Otherwise, it will be saved in the same location as the .shader file."
    )
    is_save_in_project_folder = False
    if response == QtWidgets.QMessageBox.Yes:
        is_save_in_project_folder = True
    elif response == QtWidgets.QMessageBox.Cancel:
        return
    
    # This loop collects all uniquely-identified shader items used by the materials based on its shader variant id. 
    shaderVariantIds = []
    shaderVariantListShaderOptionGroups = []
    for materialAssetId in materialAssetIds:
        materialInstanceShaderItems = azlmbr.shadermanagementconsole.ShaderManagementConsoleRequestBus(azlmbr.bus.Broadcast, 'GetMaterialInstanceShaderItems', materialAssetId)
        
        for shaderItem in materialInstanceShaderItems:
            shaderAssetId = shaderItem.GetShaderAsset().get_id()
            if shaderAssetInfo.assetId == shaderAssetId:
                shaderVariantId = shaderItem.GetShaderVariantId()
                if not shaderVariantId.IsEmpty():
                    # Check for repeat shader variant ids. We are using a list here
                    # instead of a set to check for duplicates on shaderVariantIds because
                    # shaderVariantId is not hashed by the ID like it is in the C++ side. 
                    has_repeat = False
                    for variantId in shaderVariantIds:
                        if shaderVariantId == variantId:
                            has_repeat = True
                            break
                    if has_repeat:
                        continue
                        
                    shaderVariantIds.append(shaderVariantId)
                    shaderVariantListShaderOptionGroups.append(shaderItem.GetShaderOptionGroup())
                    
    # Generate the shader variant list data by collecting shader option name-value pairs.s
    shaderVariantList = azlmbr.shader.ShaderVariantListSourceData ()
    shaderVariantList.shaderFilePath = shaderAssetInfo.relativePath
    shaderVariants = []
    stableId = 1
    for shaderOptionGroup in shaderVariantListShaderOptionGroups:
        variantInfo = azlmbr.shader.ShaderVariantInfo()
        variantInfo.stableId = stableId
        options = {}
        
        shaderOptionDescriptors = shaderOptionGroup.GetShaderOptionDescriptors()
        for shaderOptionDescriptor in shaderOptionDescriptors:
            optionName = shaderOptionDescriptor.GetName()
            optionValue = shaderOptionGroup.GetValueByOptionName(optionName)
            if not optionValue.IsValid():
                continue
                        
            valueName = shaderOptionDescriptor.GetValueName(optionValue)
            options[optionName.ToString()] = valueName.ToString()
            
        if len(options) != 0:
            variantInfo.options = options
            shaderVariants.append(variantInfo)
            stableId += 1
                
    shaderVariantList.shaderVariants = shaderVariants
    
    # clean previously generated shader variant list file so they don't clash.
    pre, ext = os.path.splitext(shaderAssetInfo.relativePath)
    projectShaderVariantListFilePath = os.path.join(azlmbr.paths.devassets, PROJECT_SHADER_VARIANTS_FOLDER, f'{pre}.shadervariantlist')
    
    pre, ext = os.path.splitext(filename)
    defaultShaderVariantListFilePath = f'{pre}.shadervariantlist'
    
    clean_existing_shadervariantlist_files([
        projectShaderVariantListFilePath
    ])
    # Save the shader variant list file
    if is_save_in_project_folder:
        shaderVariantListFilePath = projectShaderVariantListFilePath
    else:
        shaderVariantListFilePath = defaultShaderVariantListFilePath
        
    print(f"Saving .shadervariantlist file into: {shaderVariantListFilePath}")
    azlmbr.shader.SaveShaderVariantListSourceData(shaderVariantListFilePath, shaderVariantList)
    
    # Open the document in shader management console
    azlmbr.shadermanagementconsole.ShaderManagementConsoleDocumentSystemRequestBus(
        azlmbr.bus.Broadcast,
        'OpenDocument',
        shaderVariantListFilePath
    )
    
    print("==== End shader variant script ============================================================")

if __name__ == "__main__":
    main()
