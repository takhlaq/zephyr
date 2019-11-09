#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "zephyr.h"
#include "forwards.h"

int main()
{
   zephyr::Settings settings;
   settings.name = "Vulkan";
   try
   {
      zephyr::Zephyr zephyr( settings );
   }
   catch( std::exception& e )
   {
      std::cerr << e.what() << std::endl;
   }
   std::this_thread::sleep_for( 10s );
   return 0;
}