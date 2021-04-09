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

#include <Atom/RHI/RayTracingAccelerationStructure.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace AZ
{
    namespace Vulkan
    {
        class Buffer;

        //! This class builds and contains the Vulkan RayTracing TLAS buffers.
        class RayTracingTlas final
            : public RHI::RayTracingTlas
        {
        public:
            AZ_CLASS_ALLOCATOR(RayTracingTlas, AZ::SystemAllocator, 0);

            static RHI::Ptr<RayTracingTlas> Create();

            struct TlasBuffers
            {
                RHI::Ptr<RHI::Buffer> m_tlasBuffer;
                RHI::Ptr<RHI::Buffer> m_scratchBuffer;
                RHI::Ptr<RHI::Buffer> m_tlasInstancesBuffer;
                VkAccelerationStructureKHR m_accelerationStructure = VK_NULL_HANDLE;

                VkAccelerationStructureGeometryKHR m_geometry = {};
                VkAccelerationStructureBuildRangeInfoKHR m_offsetInfo = {};
                VkAccelerationStructureBuildGeometryInfoKHR m_buildInfo = {};
                uint32_t m_instanceCount = 0;
            };

            const TlasBuffers& GetBuffers() const { return m_buffers[m_currentBufferIndex]; }

            // RHI::RayTracingTlas overrides...
            virtual const RHI::Ptr<RHI::Buffer> GetTlasBuffer() const override { return m_buffers[m_currentBufferIndex].m_tlasBuffer; }

        private:
            RayTracingTlas() = default;

            // RHI::RayTracingTlas overrides
            RHI::ResultCode CreateBuffersInternal(RHI::Device& deviceBase, const RHI::RayTracingTlasDescriptor* descriptor, const RHI::RayTracingBufferPools& rayTracingBufferPools) override;

            // buffer list to keep buffers alive for several frames
            static const uint32_t BufferCount = 3;
            TlasBuffers m_buffers[BufferCount];
            uint32_t m_currentBufferIndex = 0;
        };
    }
}