#ifndef _FORWARDS_H
#define _FORWARDS_H

#include <chrono>

// todo: this is stupid but forward decl all the things gonna be pain
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vulkan/vulkan.hpp>

using namespace std::chrono_literals;

class Zephyr;

#endif