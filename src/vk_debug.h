#ifndef _ZEPHYR_UTIL_VK_DEBUG_H
#define _ZEPHYR_UTIL_VK_DEBUG_H

#include <vulkan/vulkan.h>

/*
    severity:
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes


    messageType:
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan

    pCallbackData: VkDebugUtilsMessengerCallbackDataEXT struct containing the details of the message itself, with the most important members being:
    pMessage: The debug message as a null-terminated string
    pObjects: Array of Vulkan object handles related to the message
    objectCount: Number of objects in array

    pUserData:
    pointer to pass own data to
*/

namespace zephyr::util::vk
{
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
   VkDebugUtilsMessageSeverityFlagBitsEXT severity,
   VkDebugUtilsMessageTypeFlagsEXT type,
   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   void* pUserData )
{
   std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

   return VK_FALSE;
}

/*
Extension function, not automatically loaded. We have to look up its address ourselves using vkGetInstanceProcAddr. 
Create our proxy function to handle in background. 
*/
VkResult createDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger ) {
   auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
   if( func != nullptr )
      return func( instance, pCreateInfo, pAllocator, pDebugMessenger );

   return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void destroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator )
{
   auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
   if( func != nullptr )
      return func( instance, debugMessenger, pAllocator );
}

}; // namespace zephyr::util::vk

#endif //_ZEPHYR_UTIL_VK_DEBUG_H