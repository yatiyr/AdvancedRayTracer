#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stb_image.h>


#include <Renderer.h>





int main()
{

    Renderer RnDr(128, 128);

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