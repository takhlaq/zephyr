#ifndef _ZEPHYR_H
#define _ZEPHYR_H

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

class Zephyr
{
private:
   GLFWwindow* m_pWindow;
   zephyr::Settings m_settings;
   VkInstance m_vkInstance;
   VkApplicationInfo m_vkAppInfo;
   VkDebugUtilsMessengerEXT m_vkDebugMessenger;
   VkPhysicalDevice m_vkPhysicalDevice;

   void initWindow();
   void initVulkan();
   void initDebugMessenger();
   void initPhysicalDevice();
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