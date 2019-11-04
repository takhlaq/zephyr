#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "zephyr.h"

int main()
{
   zephyr::Settings settings;
   settings.name = "Vulkan";

   zephyr::Zephyr zephyr( settings );

   return 0;
}