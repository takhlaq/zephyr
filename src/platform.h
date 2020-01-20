#ifndef _PLATFORM_H

#define _PLATFORM_H

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32 1
#include <vulkan/vulkan_win32.h>
#else
//#define GLFW_EXPOSE_NATIVE_WAYLAND
//#include <vulkan/vk_wayland.h>
#endif

#include <glfw/glfw3native.h>


#endif // _PLATFORM_H
