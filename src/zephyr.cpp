#include "zephyr.h"

#include <exception>
#include <iostream>
#include <stdexcept>

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

void Zephyr::createInstanceVulkan()
{
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
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;

      glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

      createInfo.enabledExtensionCount = glfwExtensionCount;
      createInfo.ppEnabledExtensionNames = glfwExtensions;
   }
   // layers
   createInfo.enabledLayerCount = 0;

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
   uint32_t extensionCount { 0 };
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

Zephyr::Zephyr( const ZephyrSettings& settings ) :
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
