#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "zephyr.h"

#include "vk_debug.h"

const std::vector<const char*> validationLayers =
{
   "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace zephyr
{

void Zephyr::initWindow()
{
   glfwInit();

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
   glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

   m_pWindow = glfwCreateWindow( m_settings.width, m_settings.height, m_settings.name.c_str(), nullptr, nullptr );

   if( !m_pWindow )
   {
      throw std::runtime_error( "Unable to create window." );
   }
}

void Zephyr::initVulkan()
{

}

void Zephyr::initDebugMessenger()
{
   VkDebugUtilsMessengerCreateInfoEXT createInfo;
   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
   createInfo.messageSeverity = createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
   createInfo.pfnUserCallback = zephyr::util::vk::debugCallback;
   createInfo.pUserData = nullptr;

   // vkCreateDebugUtilsMessengerEXT
   // extension func, use helper to get its address in mem and call it
   if( zephyr::util::vk::createDebugUtilsMessengerEXT( m_vkInstance, &createInfo, nullptr, &m_vkDebugMessenger ) != VK_SUCCESS )
   {
      throw std::runtime_error( "Unable to create debug messenger." );
   }
}

void Zephyr::createInstanceVulkan()
{
   if( enableValidationLayers && !checkVkValidationLayerSupport() )
   {
      throw std::runtime_error( "Unable to find all validation layers." );
   }

   m_vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   m_vkAppInfo.pApplicationName = m_settings.name.c_str();
   m_vkAppInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
   m_vkAppInfo.pEngineName = "Zephyr";
   m_vkAppInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
   m_vkAppInfo.apiVersion = VK_API_VERSION_1_0;

   VkInstanceCreateInfo createInfo;
   createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   createInfo.pApplicationInfo = &m_vkAppInfo;

   // extensions
   {
      std::vector<const char*> extensions = this->getVkRequiredExtensions();
      createInfo.enabledExtensionCount = extensions.size();
      createInfo.ppEnabledExtensionNames = extensions.data();
   }
   // layers
   if( enableValidationLayers )
   {
      createInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
      createInfo.ppEnabledLayerNames = validationLayers.data();
   }
   else
   {
      createInfo.enabledLayerCount = 0;
   }

   // create
   if( vkCreateInstance( &createInfo, nullptr, &m_vkInstance ) != VK_SUCCESS )
   {
      throw std::runtime_error( "Unable to create Vulkan instance." );
   }
}

void Zephyr::mainLoop()
{
   while( !glfwWindowShouldClose( m_pWindow ) )
   {
      glfwPollEvents();
      std::this_thread::sleep_for( 1us );
   }
}

void Zephyr::cleanup()
{
   vkDestroyInstance( m_vkInstance, nullptr );
   glfwDestroyWindow( m_pWindow );
   m_pWindow = nullptr;
   glfwTerminate();
}

const std::vector<VkExtensionProperties> Zephyr::getVkExtensions() const
{
   uint32_t extensionCount = 0;
   vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );

   std::vector<VkExtensionProperties> extensions( extensionCount );
   vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, extensions.data() );

   std::cout << "Available extensions:" << std::endl;
   for( const auto& extension : extensions )
   {
      std::cout << "\t" << extension.extensionName << std::endl;
   }
   return extensions;
}

const std::vector<const char*> Zephyr::getVkRequiredExtensions() const
{
   uint32_t glfwExtensionCount = 0;
   const char** glfwExtensions;

   glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

   std::vector<const char*> extensions( glfwExtensions, glfwExtensions + glfwExtensionCount );

   if( enableValidationLayers )
      extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

   return extensions;
}

bool Zephyr::checkVkValidationLayerSupport() const
{
   uint32_t layerCount = 0;
   vkEnumerateInstanceLayerProperties( &layerCount, nullptr );

   std::vector<VkLayerProperties> availableLayers( layerCount );
   vkEnumerateInstanceLayerProperties( &layerCount, availableLayers.data() );

   for( const char* layerName : validationLayers )
   {
      bool layerFound = false;
      for( const auto& layerProperties : availableLayers )
      {
         if( strcmp( layerProperties.layerName, layerName ) != 0 )
         {
            layerFound = true;
            break;
         }
      }

      if( !layerFound )
         return false;
   }

   return true;
}

Zephyr::Zephyr( const zephyr::Settings& settings ) :
   m_settings( settings ),
   m_pWindow( nullptr )
{
   initWindow();
   initVulkan();
   mainLoop();
   cleanup();
}

Zephyr::~Zephyr()
{
}

}; // namespace zephyr