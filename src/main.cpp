#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include <iostream>
#include <stdexcept>
//#include <functional>
//#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "game.h"

int main()
{
    Game civ;

    try
    {
        civ.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}