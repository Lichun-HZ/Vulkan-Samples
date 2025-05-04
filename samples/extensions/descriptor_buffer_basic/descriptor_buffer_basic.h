/* Copyright (c) 2023-2024, Sascha Willems
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

/*
 * Using descriptor buffers replacing descriptor sets with VK_EXT_descriptor_buffer
 * This render multiple cubes passing uniform buffers and combined image samples to the GPU using descriptor buffers instead of descriptor sets
 * This allows for a more bindless design
 */

#pragma once

#include "api_vulkan_sample.h"

class DescriptorBufferBasic : public ApiVulkanSample
{
  public:
	bool animate = true;

	VkPhysicalDeviceDescriptorBufferPropertiesEXT descriptor_buffer_properties{};

	struct DescriptorData
	{
		VkDescriptorSetLayout               layout{VK_NULL_HANDLE}; // 即使使用DescriptorBuffer，DescriptorSetLayout还是需要的
		std::unique_ptr<vkb::core::BufferC> buffer;                 // Descriptor Buffer
		VkDeviceSize                        size;                   // 通过vkGetDescriptorSetLayoutSizeEXT获取的上面layout的size，并且是descriptorBufferOffsetAlignment的整数倍
		VkDeviceSize                        offset;                 // DescriptorSetLayout中binding 0的offset
	};
	DescriptorData uniform_binding_descriptor; // layout为一个UniformBuffer，ShaderStage为Vertex
	DescriptorData image_binding_descriptor;   // layout为一个Combine Image Sampler，ShaderStage为Fragment

	struct Cube
	{
		Texture                             texture;        // 每个Cube的texture
		std::unique_ptr<vkb::core::BufferC> uniform_buffer; // 每个Cube的UBO
		glm::vec3                           rotation;
		glm::mat4                           model_mat;
	};
	std::array<Cube, 2> cubes;

	struct Models
	{
		std::unique_ptr<vkb::sg::SubMesh> cube;
	} models;

	struct UniformBuffers
	{
		std::unique_ptr<vkb::core::BufferC> scene;
	} uniform_buffers;

	struct UboScene
	{
		glm::mat4 projection;
		glm::mat4 view;
	} ubo_scene;

	VkPipeline       pipeline{VK_NULL_HANDLE};
	// 包含三个DescriptorSetLayout，set 0 = Camera UBO(Vertex), set 1 = Model UBO(Vertex) and set 2 = Model combined image(Fragment)
	VkPipelineLayout pipeline_layout{VK_NULL_HANDLE};

	DescriptorBufferBasic();
	~DescriptorBufferBasic() override;
	virtual void request_gpu_features(vkb::PhysicalDevice &gpu) override;
	void         build_command_buffers() override;
	void         load_assets();
	void         setup_descriptor_set_layout();
	void         prepare_pipelines();
	void         prepare_descriptor_buffer();
	void         prepare_uniform_buffers();
	void         update_uniform_buffers();
	void         update_cube_uniform_buffers(float delta_time);
	void         draw();
	bool         prepare(const vkb::ApplicationOptions &options) override;
	void         render(float delta_time) override;
	void         on_update_ui_overlay(vkb::Drawer &drawer) override;
};

std::unique_ptr<vkb::VulkanSampleC> create_descriptor_buffer_basic();
