#ifndef _ZEPHYR_H
#define _ZEPHYR_H

#include <optional>
#include <string>
#include <vector>

#include "forwards.h"

namespace zephyr
{
struct Settings
{
   int width { 800 };
   int height { 600 };
   std::string name;
   int monitorIndex { 0 };
};
struct QueueFamilyIndices
{
   std::optional<uint32_t> graphicsFamily;
   std::optional<uint32_t> presentFamily;
   bool isComplete()
   {
      return graphicsFamily.has_value() && presentFamily.has_value();
   }
};

class Zephyr
{
private:

   GLFWwindow* m_pWindow;
   zephyr::Settings m_settings;
   VkInstance m_vkInstance;
   VkApplicationInfo m_vkAppInfo;
   VkDebugUtilsMessengerEXT m_vkDebugMessenger;
   VkSurfaceKHR m_vkSurface;
   VkPhysicalDevice m_vkPhysicalDevice;
   VkDevice m_vkLogicalDevice;
   VkQueue m_vkGraphicsQueue;
   VkQueue m_vkPresentQueue;

   void initWindow();
   void initVulkan();
   void initDebugMessenger();
   void createSurface();
   QueueFamilyIndices findQueueFamilies( VkPhysicalDevice device );
   bool isDeviceSuitable( VkPhysicalDevice device );
   int rateDeviceSuitability( VkPhysicalDevice device );
   void pickPhysicalDevice();
   void createLogicalDevice();
   void createInstanceVulkan();
   
   void cleanup();

   void mainLoop();
public:

   static const std::vector<VkExtensionProperties> getVkExtensions();
   static const std::vector<const char*> getVkRequiredExtensions();

   bool checkVkValidationLayerSupport() const;

   Zephyr( const zephyr::Settings& );
   Zephyr( const Zephyr& ) = delete;
   Zephyr( const Zephyr&& ) = delete;

   ~Zephyr();
};
};

#endif //_ZEPHYR_H