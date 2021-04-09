/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 *
 * For complete copyright and license terms please see the LICENSE at the root of this
 * distribution (the "License"). All use of this software is governed by the License,
 * or, if provided, by the license below or the license accompanying this file. Do not
 * remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 */
#pragma once

namespace AWSCore
{
    class AWSCoreEditorMenu;

    class AWSCoreEditorManager
    {
    public:
        static constexpr const char CLOUD_SERVICES_MENU_TEXT[] = "&Cloud services";

        AWSCoreEditorManager();
        virtual ~AWSCoreEditorManager();

        //! Get AWSCoreEditorMenu UI component
        AWSCoreEditorMenu* GetAWSCoreEditorMenu() const;

    private:
        AWSCoreEditorMenu* m_awsCoreEditorMenu;
    };
} // namespace AWSCore