#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <thread>

#include "zephyr.h"
#include "vk_debug.h"

#include "platform.h"

const std::vector<const char*> validationLayers =
{
   "VK_LAYER_KHRONOS_validation",
   "VK_LAYER_LUNARG_monitor",
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
   createInstanceVulkan();
   initDebugMessenger();
   createSurface();
   pickPhysicalDevice();
   createLogicalDevice();
}

void Zephyr::initDebugMessenger()
{
   if( !enableValidationLayers ) return;

   VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

   zephyr::util::vk::populateDebugMessengerCreateInfo( createInfo );

   // vkCreateDebugUtilsMessengerEXT
   // extension func, use helper to get its address in mem and call it
   if( zephyr::util::vk::createDebugUtilsMessengerEXT( m_vkInstance, &createInfo, nullptr, &m_vkDebugMessenger ) != VK_SUCCESS )
   {
      throw std::runtime_error( "Unable to create debug messenger." );
   }
}

void Zephyr::createSurface()
{
   if( glfwCreateWindowSurface( m_vkInstance, m_pWindow, nullptr, &m_vkSurface ) != VK_SUCCESS )
   {
      throw std::runtime_error( "Failed to create window surface." );
   }
}

QueueFamilyIndices Zephyr::findQueueFamilies( VkPhysicalDevice device )
{
   QueueFamilyIndices indices;
   // assign index to queue families that could be found
   uint32_t queueFamilyCount = 0;
   vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );

   std::vector<VkQueueFamilyProperties> queueFamilies( queueFamilyCount );
   vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data() );


   int i = 0;

   for( const auto& queueFamily : queueFamilies )
   {
      if( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
         indices.graphicsFamily = i;

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR( device, i, m_vkSurface, &presentSupport );
      if( presentSupport )
         indices.presentFamily = i;

      if( indices.isComplete() )
         break;
      ++i;
   }

   return indices;
}

bool Zephyr::isDeviceSuitable( VkPhysicalDevice device )
{
   QueueFamilyIndices indices = findQueueFamilies( device );

   return indices.isComplete();
}

int Zephyr::rateDeviceSuitability( VkPhysicalDevice device )
{
   VkPhysicalDeviceProperties deviceProps;
   VkPhysicalDeviceFeatures deviceFeatures;
   vkGetPhysicalDeviceProperties( device, &deviceProps );
   vkGetPhysicalDeviceFeatures( device, &deviceFeatures );

   if( !isDeviceSuitable( device ) )
      return 0;

   if( !deviceFeatures.geometryShader )
      return 0;

   int score = 0;

   if( deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
      score += 1000;

   score += deviceProps.limits.maxImageDimension2D;
}

void Zephyr::pickPhysicalDevice()
{
   m_vkPhysicalDevice = VK_NULL_HANDLE;

   uint32_t deviceCount = 0;
   vkEnumeratePhysicalDevices( m_vkInstance, &deviceCount, nullptr );

   if( deviceCount == 0 )
      throw std::runtime_error( "Failed to find GPUs with Vulkan support." );

   std::vector<VkPhysicalDevice> devices( deviceCount );
   vkEnumeratePhysicalDevices( m_vkInstance, &deviceCount, devices.data() );

   std::multimap<int, VkPhysicalDevice> candidates;

   for( const auto& device : devices )
   {
      int score = rateDeviceSuitability( device );
      if( score != 0 )
         candidates.insert( std::make_pair( score, device ) );
   }

   if( candidates.rbegin()->first )
      m_vkPhysicalDevice = candidates.rbegin()->second;
   else
      throw std::runtime_error( "Unable to find suitable GPU." );

}

void Zephyr::createLogicalDevice()
{
   QueueFamilyIndices indices = findQueueFamilies( m_vkPhysicalDevice );
   
   std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
   std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

   float queuePriority = 1.0f;

   for( uint32_t queueFamily : uniqueQueueFamilies )
   {
      VkDeviceQueueCreateInfo queueCreateInfo = {};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back( queueCreateInfo );
   }
   

   VkPhysicalDeviceFeatures deviceFeatures = {};
   VkDeviceCreateInfo createInfo = {};

   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
   createInfo.pQueueCreateInfos = queueCreateInfos.data();
   createInfo.queueCreateInfoCount = static_cast<uint32_t>( queueCreateInfos.size() );

   createInfo.pEnabledFeatures = &deviceFeatures;
   createInfo.enabledExtensionCount = 0;

   if( enableValidationLayers )
   {
      createInfo.enabledLayerCount = validationLayers.size();
      createInfo.ppEnabledLayerNames = validationLayers.data();
   }
   else
   {
      createInfo.enabledLayerCount = 0;
   }

   if( vkCreateDevice( m_vkPhysicalDevice, &createInfo, nullptr, &m_vkLogicalDevice ) != VK_SUCCESS )
   {
      throw std::runtime_error( "Unable to create logical device." );
   }

   vkGetDeviceQueue( m_vkLogicalDevice, indices.graphicsFamily.value(), 0, &m_vkGraphicsQueue );
   vkGetDeviceQueue( m_vkLogicalDevice, indices.presentFamily.value(), 0, &m_vkPresentQueue );
}

void Zephyr::createInstanceVulkan()
{
   if( enableValidationLayers && !checkVkValidationLayerSupport() )
   {
      throw std::runtime_error( "Unable to find all validation layers." );
   }

   m_vkAppInfo = {};

   m_vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   m_vkAppInfo.pApplicationName = m_settings.name.c_str();
   m_vkAppInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
   m_vkAppInfo.pEngineName = "Zephyr";
   m_vkAppInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
   m_vkAppInfo.apiVersion = VK_API_VERSION_1_0;

   VkInstanceCreateInfo createInfo = {};
   //memset( &createInfo, 0, sizeof( createInfo) );

   createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   createInfo.pApplicationInfo = &m_vkAppInfo;

   // extensions
   std::vector<const char*> extensions = this->getVkRequiredExtensions();
   {
      createInfo.enabledExtensionCount = extensions.size();
      createInfo.ppEnabledExtensionNames = extensions.data();
   }
   // layers
   VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
   if( enableValidationLayers )
   {
      createInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
      createInfo.ppEnabledLayerNames = validationLayers.data();

      zephyr::util::vk::populateDebugMessengerCreateInfo( debugCreateInfo );
      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)& debugCreateInfo;
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
   zephyr::util::vk::destroyDebugUtilsMessengerEXT( m_vkInstance, m_vkDebugMessenger, nullptr );
   vkDestroyDevice( m_vkLogicalDevice, nullptr );

   vkDestroySurfaceKHR( m_vkInstance, m_vkSurface, nullptr );
   vkDestroyInstance( m_vkInstance, nullptr );

   glfwDestroyWindow( m_pWindow );
   glfwTerminate();
}

const std::vector<VkExtensionProperties> Zephyr::getVkExtensions()
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

const std::vector<const char*> Zephyr::getVkRequiredExtensions()
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