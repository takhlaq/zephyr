#include "zephyr.h"

#include <exception>
#include <stdexcept>

void Zephyr::initWindow()
{
   glfwInit();

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
   glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

   m_window = glfwCreateWindow( m_settings.width, m_settings.height, m_settings.name.c_str(), nullptr, nullptr );

   if( !m_window )
   {
      throw std::runtime_error( "Unable to create window." );
   }
}

void Zephyr::initVulkan()
{
}

void Zephyr::mainLoop()
{
   while( !glfwWindowShouldClose( m_window ) )
   {
      glfwPollEvents();
   }
}

void Zephyr::cleanup()
{
   glfwDestroyWindow( m_window );
   m_window = nullptr;
   glfwTerminate();
}

Zephyr::Zephyr( const ZephyrSettings& settings ) :
   m_settings( settings ),
   m_window( nullptr )
{
   initWindow();
   initVulkan();
   mainLoop();
   cleanup();
}

Zephyr::~Zephyr()
{
}
