/* Copyright (c) 2021-2024, Arm Limited and Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "api_vulkan_sample.h"
#include <random>
#include <vector>

class DescriptorIndexing : public ApiVulkanSample
{
  public:
	DescriptorIndexing();
	~DescriptorIndexing();

  private:
	virtual void request_gpu_features(vkb::PhysicalDevice &gpu) override;
	virtual void render(float delta_time) override;
	virtual void build_command_buffers() override;
	virtual void on_update_ui_overlay(vkb::Drawer &drawer) override;
	virtual bool prepare(const vkb::ApplicationOptions &options) override;

	void create_bindless_descriptors();
	void create_immutable_sampler_descriptor_set();
	void create_pipelines();

	// 支持update_after_bind Descriptor
	struct DescriptorHeap
	{
		VkDescriptorSetLayout set_layout{};                       // fragment, binding 0, Sampled Image
		VkDescriptorPool      descriptor_pool{};
		VkDescriptorSet       descriptor_set_update_after_bind{}; // 2048个Descriptor，用于Update after bind
		VkDescriptorSet       descriptor_set_nonuniform{};        // 64个Descriptor，用于non uniform index
	} descriptors;

	// 普通Descriptor
	struct ImmutableSampler
	{
		VkSampler             sampler{};                          // fragment, binding 0, Sampler
		VkDescriptorSetLayout set_layout{};
		VkDescriptorPool      descriptor_pool{};
		VkDescriptorSet       descriptor_set{};
	} sampler;

	struct Pipelines
	{
		VkPipelineLayout pipeline_layout{};      // set 0: descriptors.set_layout; set 1: sampler.set_layout
		// 两个pipeline其他都相同，只是使用不同的shader
		VkPipeline       update_after_bind{};    // "update-after-bind-quads.vert", "update-after-bind-quads.frag"
		VkPipeline       non_uniform_indexing{}; // "nonuniform-quads.vert", "nonuniform-quads.frag"
	} pipelines;

	struct TestImage
	{
		VkImage        image{};
		VkImageView    image_view{};
		VkDeviceMemory memory{};
	};
	std::vector<TestImage> test_images;
	void                   create_images();
	TestImage              create_image(const float rgb[3], unsigned image_seed);

	VkPhysicalDeviceDescriptorIndexingPropertiesEXT descriptor_indexing_properties{};
	std::default_random_engine                      rnd{42};
	std::uniform_real_distribution<float>           distribution{0.0f, 0.1f};
	uint32_t                                        descriptor_offset{};
	float                                           accumulated_time{};
	const VkFormat                                  format = VK_FORMAT_R8G8B8A8_UNORM;
};

std::unique_ptr<vkb::VulkanSampleC> create_descriptor_indexing();
