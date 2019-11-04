#ifndef _ZEPHYR_H
#define _ZEPHYR_H

#include <string>

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
   GLFWwindow* m_window;
   ZephyrSettings m_settings;

   void initWindow();
   void initVulkan();
   void mainLoop();
   void cleanup();
public:
   Zephyr(const ZephyrSettings&);
   ~Zephyr();
};

#endif //_ZEPHYR_H