#ifndef _ZEPHYR_H
#define _ZEPHYR_H

#include <string>
#include <vector>

#include "forwards.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>

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

   void initWindow();
   void initVulkan();
   void initDebugMessenger();
   void createInstanceVulkan();
   
   void mainLoop();
public:
   void cleanup();

   const std::vector<VkExtensionProperties> getVkExtensions() const;
   const std::vector<const char*> getVkRequiredExtensions() const;

   bool checkVkValidationLayerSupport() const;

   Zephyr(const zephyr::Settings&);
   ~Zephyr();
};
};

#endif //_ZEPHYR_H