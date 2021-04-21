#include <iostream>
#include <Renderer.h>
#include <Timer.h>


int main(int argc, char** argv)
{

    Renderer RnDr("assets/scenes/car.xml", RenderingMode::OFFSCREEN_RENDERING_BIT);

    try
    {    
        RnDr.OneTimeRender();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    return 0;
}