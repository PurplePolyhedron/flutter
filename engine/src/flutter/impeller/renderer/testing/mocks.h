// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_IMPELLER_RENDERER_TESTING_MOCKS_H_
#define FLUTTER_IMPELLER_RENDERER_TESTING_MOCKS_H_

#include "gmock/gmock.h"
#include "impeller/core/allocator.h"
#include "impeller/core/sampler_descriptor.h"
#include "impeller/core/texture.h"
#include "impeller/renderer/command_buffer.h"
#include "impeller/renderer/command_queue.h"
#include "impeller/renderer/context.h"
#include "impeller/renderer/render_pass.h"
#include "impeller/renderer/render_target.h"
#include "impeller/renderer/sampler_library.h"

namespace impeller {
namespace testing {

class MockDeviceBuffer : public DeviceBuffer {
 public:
  explicit MockDeviceBuffer(const DeviceBufferDescriptor& desc)
      : DeviceBuffer(desc) {}

  MOCK_METHOD(bool, SetLabel, (const std::string& label), (override));

  MOCK_METHOD(bool,
              SetLabel,
              (const std::string& label, Range range),
              (override));

  MOCK_METHOD(uint8_t*, OnGetContents, (), (const, override));

  MOCK_METHOD(bool,
              OnCopyHostBuffer,
              (const uint8_t* source, Range source_range, size_t offset),
              (override));
};

class MockAllocator : public Allocator {
 public:
  MOCK_METHOD(ISize, GetMaxTextureSizeSupported, (), (const, override));
  MOCK_METHOD(std::shared_ptr<DeviceBuffer>,
              OnCreateBuffer,
              (const DeviceBufferDescriptor& desc),
              (override));
  MOCK_METHOD(std::shared_ptr<Texture>,
              OnCreateTexture,
              (const TextureDescriptor& desc),
              (override));
};

class MockBlitPass : public BlitPass {
 public:
  MOCK_METHOD(bool, IsValid, (), (const, override));
  MOCK_METHOD(bool,
              EncodeCommands,
              (const std::shared_ptr<Allocator>& transients_allocator),
              (const, override));
  MOCK_METHOD(void, OnSetLabel, (std::string label), (override));

  MOCK_METHOD(bool,
              OnCopyTextureToTextureCommand,
              (std::shared_ptr<Texture> source,
               std::shared_ptr<Texture> destination,
               IRect source_region,
               IPoint destination_origin,
               std::string label),
              (override));

  MOCK_METHOD(bool,
              OnCopyTextureToBufferCommand,
              (std::shared_ptr<Texture> source,
               std::shared_ptr<DeviceBuffer> destination,
               IRect source_region,
               size_t destination_offset,
               std::string label),
              (override));
  MOCK_METHOD(bool,
              OnCopyBufferToTextureCommand,
              (BufferView source,
               std::shared_ptr<Texture> destination,
               IPoint destination_origin,
               std::string label),
              (override));
  MOCK_METHOD(bool,
              OnGenerateMipmapCommand,
              (std::shared_ptr<Texture> texture, std::string label),
              (override));
};

class MockRenderPass : public RenderPass {
 public:
  MockRenderPass(std::shared_ptr<const Context> context,
                 const RenderTarget& target)
      : RenderPass(std::move(context), target) {}
  MOCK_METHOD(bool, IsValid, (), (const, override));
  MOCK_METHOD(bool,
              OnEncodeCommands,
              (const Context& context),
              (const, override));
  MOCK_METHOD(void, OnSetLabel, (std::string label), (override));
};

class MockCommandBuffer : public CommandBuffer {
 public:
  explicit MockCommandBuffer(std::weak_ptr<const Context> context)
      : CommandBuffer(std::move(context)) {}
  MOCK_METHOD(bool, IsValid, (), (const, override));
  MOCK_METHOD(void, SetLabel, (const std::string& label), (const, override));
  MOCK_METHOD(std::shared_ptr<BlitPass>, OnCreateBlitPass, (), (override));
  MOCK_METHOD(bool,
              OnSubmitCommands,
              (CompletionCallback callback),
              (override));
  MOCK_METHOD(void, OnWaitUntilScheduled, (), (override));
  MOCK_METHOD(std::shared_ptr<ComputePass>,
              OnCreateComputePass,
              (),
              (override));
  MOCK_METHOD(std::shared_ptr<RenderPass>,
              OnCreateRenderPass,
              (RenderTarget render_target),
              (override));
};

class MockImpellerContext : public Context {
 public:
  MOCK_METHOD(Context::BackendType, GetBackendType, (), (const, override));

  MOCK_METHOD(std::string, DescribeGpuModel, (), (const, override));

  MOCK_METHOD(bool, IsValid, (), (const, override));

  MOCK_METHOD(void, Shutdown, (), (override));

  MOCK_METHOD(std::shared_ptr<Allocator>,
              GetResourceAllocator,
              (),
              (const, override));

  MOCK_METHOD(std::shared_ptr<ShaderLibrary>,
              GetShaderLibrary,
              (),
              (const, override));

  MOCK_METHOD(std::shared_ptr<SamplerLibrary>,
              GetSamplerLibrary,
              (),
              (const, override));

  MOCK_METHOD(std::shared_ptr<PipelineLibrary>,
              GetPipelineLibrary,
              (),
              (const, override));

  MOCK_METHOD(std::shared_ptr<CommandBuffer>,
              CreateCommandBuffer,
              (),
              (const, override));

  MOCK_METHOD(const std::shared_ptr<const Capabilities>&,
              GetCapabilities,
              (),
              (const, override));

  MOCK_METHOD(std::shared_ptr<CommandQueue>,
              GetCommandQueue,
              (),
              (const, override));
};

class MockTexture : public Texture {
 public:
  explicit MockTexture(const TextureDescriptor& desc) : Texture(desc) {}
  MOCK_METHOD(void, SetLabel, (std::string_view label), (override));
  MOCK_METHOD(bool, IsValid, (), (const, override));
  MOCK_METHOD(ISize, GetSize, (), (const, override));
  MOCK_METHOD(bool,
              OnSetContents,
              (const uint8_t* contents, size_t length, size_t slice),
              (override));
  MOCK_METHOD(bool,
              OnSetContents,
              (std::shared_ptr<const fml::Mapping> mapping, size_t slice),
              (override));
};

class MockCapabilities : public Capabilities {
 public:
  MOCK_METHOD(bool, SupportsOffscreenMSAA, (), (const, override));
  MOCK_METHOD(bool, SupportsImplicitResolvingMSAA, (), (const, override));
  MOCK_METHOD(bool, SupportsSSBO, (), (const, override));
  MOCK_METHOD(bool, SupportsBufferToTextureBlits, (), (const, override));
  MOCK_METHOD(bool, SupportsTextureToTextureBlits, (), (const, override));
  MOCK_METHOD(bool, SupportsFramebufferFetch, (), (const, override));
  MOCK_METHOD(bool, SupportsCompute, (), (const, override));
  MOCK_METHOD(bool, SupportsComputeSubgroups, (), (const, override));
  MOCK_METHOD(bool, SupportsReadFromResolve, (), (const, override));
  MOCK_METHOD(bool, SupportsDecalSamplerAddressMode, (), (const, override));
  MOCK_METHOD(bool, SupportsDeviceTransientTextures, (), (const, override));
  MOCK_METHOD(PixelFormat, GetDefaultColorFormat, (), (const, override));
  MOCK_METHOD(PixelFormat, GetDefaultStencilFormat, (), (const, override));
  MOCK_METHOD(PixelFormat, GetDefaultDepthStencilFormat, (), (const, override));
  MOCK_METHOD(PixelFormat, GetDefaultGlyphAtlasFormat, (), (const, override));
};

class MockCommandQueue : public CommandQueue {
 public:
  MOCK_METHOD(fml::Status,
              Submit,
              (const std::vector<std::shared_ptr<CommandBuffer>>& buffers,
               const CompletionCallback& cb),
              (override));
};

class MockSamplerLibrary : public SamplerLibrary {
 public:
  MOCK_METHOD(const std::unique_ptr<const Sampler>&,
              GetSampler,
              (SamplerDescriptor descriptor),
              (override));
};

class MockSampler : public Sampler {
 public:
  explicit MockSampler(const SamplerDescriptor& desc) : Sampler(desc) {}
};

}  // namespace testing
}  // namespace impeller

#endif  // FLUTTER_IMPELLER_RENDERER_TESTING_MOCKS_H_
