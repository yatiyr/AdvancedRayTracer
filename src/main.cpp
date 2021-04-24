#include <iostream>
#include <Renderer.h>
#include <Timer.h>


int main(int argc, char** argv)
{

    Renderer RnDr("assets/scenes/hw1/simple.xml", RenderingMode::CPU_RENDERING_BIT);

    try
    {    
        RnDr.CPURender();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    return 0;
}