#include <iostream>
#include <Renderer.h>
#include <Timer.h>


int main()
{

    Renderer RnDr;

    try
    {    
        {
        Timer t;
        RnDr.OneTimeRender();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    return 0;
}