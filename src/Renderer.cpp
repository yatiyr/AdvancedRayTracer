#include <Renderer.h>



void Renderer::Framebuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// TODO: Will be implemented
void Renderer::Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{

}

// TODO: Will be implemented
void Renderer::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

// TODO: Will be implemented
void Renderer::Process_Input(GLFWwindow* window)
{
    // CLOSE THE WINDOW IF WE PRESS ESC
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

void Renderer::LoadFunctionPointers()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Could not load function pointers.");
    }
}


void Renderer::PrintDeviceProperties()
{
    std::cout << "OpenGL:\n";
    std::cout << "\tvendor          => " << glGetString(GL_VENDOR)   << '\n';
    std::cout << "\trenderer        => " << glGetString(GL_RENDERER) << '\n';
    std::cout << "\tversion         => " << glGetString(GL_VERSION)  << '\n';
    std::cout << "\tshader language => " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}

void Renderer::FindComputeShaderExtension()
{
    int extensionCount;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
    bool found = false;
    for(size_t i=0; i<extensionCount; i++)
    {
        if(!strcmp((const char*)glGetStringi(GL_EXTENSIONS, i), "GL_ARB_compute_shader"))
        {
            found = true;
        }
    }

    if(!found)
    {
        throw std::runtime_error("Extension \"GL_ARB_compute_shader\" not found");
    }
}

void Renderer::InitializeGL(int width, int height)
{
    glfwInit();

    // Use OpenGL 4.6 as contest
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif      

}

Renderer::Renderer(unsigned int initialWidth, unsigned int initialHeight)
{
    _width  = initialWidth;
    _height = initialHeight;

    InitializeGL(_width, _height);


    _window = glfwCreateWindow(_width, _height, "AdvancedRayTracer", nullptr, nullptr);
    if(_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Could not create glfw window.");

    }     

    // Change contex to our window class variable
    glfwMakeContextCurrent(_window);
    

    // Register callbacks
    glfwSetFramebufferSizeCallback(_window, Framebuffer_Size_Callback);
    glfwSetCursorPosCallback(_window, Mouse_Callback);
    glfwSetScrollCallback(_window, Scroll_Callback);

    // if we want to disable mouse
    // glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    LoadFunctionPointers();

    GenerateTexture();

    PrintDeviceProperties();
    FindComputeShaderExtension();

    glViewport(0, 0, _width, _height);

    // read load compile and link shader programs
    InitializeShaderPrograms();


}

void Renderer::InitializeShaderPrograms()
{
    std::string vertexShaderPath   = std::string(ROOT_DIR) + "src/shaders/VertexShader.vert";
    std::string fragmentShaderPath = std::string(ROOT_DIR) + "src/shaders/FragmentShader.frag";
    std::string computeShaderPath  = std::string(ROOT_DIR) + "src/shaders/ComputeShader.comp";

    _renderProgram  = new RenderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    _computeProgram = new ComputeProgram(computeShaderPath.c_str());

}

void Renderer::RenderLoop()
{
    while(!glfwWindowShouldClose(_window))
    {

        // calculate deltatime
        float currentFrame = glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        Process_Input(_window);


        UpdateTexture(currentFrame);
        DrawImage();



        // check and call events and callback functions
        // if we have defined them
        glfwPollEvents();
    }

    glDeleteProgram(_computeProgram->id);
    glDeleteProgram(_renderProgram->id);

    glfwTerminate();
}

void Renderer::GenerateTexture()
{
    glGenTextures(1, &_textureHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, 0);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // in order to write to this texture we bind it to an image unit
    glBindImageTexture(0, _textureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Renderer::UpdateTexture(float frame)
{

    glUseProgram(_computeProgram->id);
	glUniform1f(glGetUniformLocation(_computeProgram->id, "roll"), frame);    
    //_computeProgram->SetFloat("roll", (float)frame*0.01f);

    // _width * _height threads in blocks of 16^2
    glDispatchCompute(_width/16, _height/16, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    
}

void Renderer::DrawImage()
{
    glUseProgram(_renderProgram->id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glfwSwapBuffers(_window);
}

Renderer::~Renderer()
{

    delete _renderProgram;
    delete _computeProgram;

}
