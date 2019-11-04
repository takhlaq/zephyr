#ifndef _ZEPHYR_H
#define _ZEPHYR_H

#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>

struct ZephyrSettings
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
   ZephyrSettings m_settings;
   VkInstance m_vkInstance;
   VkApplicationInfo m_vkAppInfo;

   void initWindow();
   void initVulkan();
   void createInstanceVulkan();
   void mainLoop();
public:
   void cleanup();

   const std::vector<VkExtensionProperties> getVkExtensions() const;

   Zephyr(const ZephyrSettings&);
   ~Zephyr();
};

#endif //_ZEPHYR_H