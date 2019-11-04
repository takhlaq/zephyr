#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "zephyr.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

int main()
{
   zephyr::Settings settings;
   settings.name = "Vulkan";

   zephyr::Zephyr zephyr( settings );

   return 0;
}