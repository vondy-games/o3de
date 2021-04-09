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

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/utils.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/typetraits/typetraits.h>

namespace AzToolsFramework::Prefab::PrefabConversionUtils
{
    //! Storage for objects created through the Prefab processing pipeline.
    //! These typically store the created object for immediate use in the editor plus additional information
    //! to allow the Prefab Builder to convert the object into a serialized form and register it with the
    //! Asset Database.
    class ProcessedObjectStore
    {
    public:
        using SerializerFunction = AZStd::function<bool(AZStd::vector<uint8_t>&, const ProcessedObjectStore&)>;

        //! Constructs a new instance.
        //! @param uniqueId A name for the object that's unique within the scope of the Prefab. This name will be used to generate a sub id for the product
        //!     which requires that the name to be stable between runs.
        //! @param sourceId The uuid for the source file.
        //! @param assetSerializer The callback used to convert the provided asset into a binary version.
        template<typename T>
        static AZStd::pair<ProcessedObjectStore, T*> Create(AZStd::string uniqueId, const AZ::Uuid& sourceId,
            SerializerFunction assetSerializer);
        
        bool Serialize(AZStd::vector<uint8_t>& output) const;
        static uint32_t BuildSubId(AZStd::string_view id);

        bool HasAsset() const;
        const AZ::Data::AssetType& GetAssetType() const;
        const AZ::Data::AssetData& GetAsset() const;
        AZ::Data::AssetData& GetAsset();
        AZStd::unique_ptr<AZ::Data::AssetData> ReleaseAsset();

        const AZStd::string& GetId() const;

    private:
        ProcessedObjectStore(AZStd::string uniqueId, AZStd::unique_ptr<AZ::Data::AssetData> asset, SerializerFunction assetSerializer);

        SerializerFunction m_assetSerializer;
        AZStd::unique_ptr<AZ::Data::AssetData> m_asset;
        AZStd::string m_uniqueId;
    };

    template<typename T>
    AZStd::pair<ProcessedObjectStore, T*> ProcessedObjectStore::Create(AZStd::string uniqueId, const AZ::Uuid& sourceId,
        SerializerFunction assetSerializer)
    {
        static_assert(AZStd::is_base_of_v<AZ::Data::AssetData, T>,
            "ProcessedObjectStore can only be created from a class that derives from AZ::Data::AssetData.");
        AZ::Data::AssetId assetId(sourceId, BuildSubId(uniqueId));
        auto instance = AZStd::make_unique<T>(assetId, AZ::Data::AssetData::AssetStatus::Ready);
        ProcessedObjectStore resultLeft(AZStd::move(uniqueId), AZStd::move(instance), AZStd::move(assetSerializer));
        T* resultRight = static_cast<T*>(&resultLeft.GetAsset());
        return AZStd::make_pair<ProcessedObjectStore, T*>(AZStd::move(resultLeft), resultRight);
    }
} // namespace AzToolsFramework::Prefab::PrefabConversionUtils