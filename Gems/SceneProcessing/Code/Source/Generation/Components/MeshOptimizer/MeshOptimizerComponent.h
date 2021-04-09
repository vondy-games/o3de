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

#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/tuple.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Memory/Memory.h>
#include <AzCore/RTTI/RTTI.h>
#include <SceneAPI/SceneCore/Components/GenerationComponent.h>
#include <SceneAPI/SceneCore/Containers/SceneGraph.h>
#include <SceneAPI/SceneCore/Events/ProcessingResult.h>

namespace AZ { class ReflectContext; }
namespace AZ::SceneAPI::DataTypes { class IMeshData; }
namespace AZ::SceneAPI::DataTypes { class IMeshGroup; }
namespace AZ::SceneAPI::DataTypes { class IMeshVertexUVData; }
namespace AZ::SceneAPI::DataTypes { class IMeshVertexTangentData; }
namespace AZ::SceneAPI::DataTypes { class IMeshVertexBitangentData; }
namespace AZ::SceneAPI::DataTypes { class ISkinWeightData; }
namespace AZ::SceneAPI::DataTypes { class IMeshVertexColorData; }
namespace AZ::SceneAPI::Events { class GenerateSimplificationEventContext; }
namespace AZ::SceneData::GraphData { class MeshVertexBitangentData; }
namespace AZ::SceneData::GraphData { class MeshVertexColorData; }
namespace AZ::SceneData::GraphData { class MeshVertexTangentData; }
namespace AZ::SceneData::GraphData { class MeshVertexUVData; }
namespace AZ::SceneAPI::Containers { class SceneGraph; }

namespace AZ::SceneGenerationComponents
{
    class MeshOptimizerComponent
        : public AZ::SceneAPI::SceneCore::GenerationComponent
    {
    public:
        AZ_COMPONENT(MeshOptimizerComponent, "{05791580-A464-436C-B3EA-36AD68A42BC8}", AZ::SceneAPI::SceneCore::GenerationComponent)

        MeshOptimizerComponent();

        static void Reflect(AZ::ReflectContext* context);

        AZ::SceneAPI::Events::ProcessingResult OptimizeMeshes(AZ::SceneAPI::Events::GenerateSimplificationEventContext& context) const;

        static bool HasAnyBlendShapeChild(const AZ::SceneAPI::Containers::SceneGraph& graph, const AZ::SceneAPI::Containers::SceneGraph::NodeIndex& nodeIndex);

    private:
        static AZStd::tuple<
            AZStd::unique_ptr<AZ::SceneAPI::DataTypes::IMeshData>,
            AZStd::vector<AZStd::unique_ptr<AZ::SceneData::GraphData::MeshVertexUVData>>,
            AZStd::vector<AZStd::unique_ptr<AZ::SceneData::GraphData::MeshVertexTangentData>>,
            AZStd::vector<AZStd::unique_ptr<AZ::SceneData::GraphData::MeshVertexBitangentData>>,
            AZStd::vector<AZStd::unique_ptr<AZ::SceneData::GraphData::MeshVertexColorData>>,
            AZStd::unique_ptr<AZ::SceneAPI::DataTypes::ISkinWeightData>
        > OptimizeMesh(
            const AZ::SceneAPI::DataTypes::IMeshData* meshData,
            const AZStd::vector<AZStd::reference_wrapper<const AZ::SceneAPI::DataTypes::IMeshVertexUVData>>& uvs,
            const AZStd::vector<AZStd::reference_wrapper<const AZ::SceneAPI::DataTypes::IMeshVertexTangentData>>& tangents,
            const AZStd::vector<AZStd::reference_wrapper<const AZ::SceneAPI::DataTypes::IMeshVertexBitangentData>>& bitangents,
            const AZStd::vector<AZStd::reference_wrapper<const AZ::SceneAPI::DataTypes::IMeshVertexColorData>>& vertexColors,
            const AZStd::vector<AZStd::reference_wrapper<const AZ::SceneAPI::DataTypes::ISkinWeightData>>& skinWeights,
            const AZ::SceneAPI::DataTypes::IMeshGroup& meshGroup,
            bool hasBlendShapes);
    };
} // namespace AZ::SceneGenerationComponents