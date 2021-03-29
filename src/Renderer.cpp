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

    GetMaximumWorkGroupCount();

    GetMaximumWorkGroupSize();

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

        glfwSwapBuffers(_window);        
    }

    glDeleteProgram(_computeProgram->id);
    glDeleteProgram(_renderProgram->id);

    glfwTerminate();
}

void Renderer::GetMaximumWorkGroupCount()
{
    int work_group_count[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_count[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_count[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_count[2]);

    std::cout << "work group counts x: " << work_group_count[0] << " y: " << work_group_count[1] << " z: " << work_group_count[2] << '\n';
}

void Renderer::GetMaximumWorkGroupSize()
{
    int work_group_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);

    std::cout << "work group size x: " << work_group_size[0] << " y: " << work_group_size[1] << " z: " << work_group_size[2] << '\n';
}

void Renderer::GenerateTexture()
{
    glGenTextures(1, &_textureHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, 0);

    // in order to write to this texture we bind it to an image unit
    glBindImageTexture(0, _textureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Renderer::UpdateTexture(float frame)
{

    glActiveTexture(GL_TEXTURE0);
    glUseProgram(_computeProgram->id);  
    //_computeProgram->SetFloat("roll", (float)frame*0.01f);

    // _width * _height threads in blocks of 16^2
    glDispatchCompute(_width, _height, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    
}

void Renderer::DrawImage()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_renderProgram->id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

uint8_t* Renderer::FloatToUint8(float* pixels)
{
    // we will omit alpha channel
    uint8_t* result = new uint8_t[_width * _height * 3];

    int indexSrc = 0;
    int indexDst = 0;
    for(int j= 0; j<_height; j++)
    {
        for(int i=0; i<_width; i++)
        {
            float r = pixels[indexSrc++];
            float g = pixels[indexSrc++];
            float b = pixels[indexSrc++];
            indexSrc++;

            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            result[indexDst++] = ir;
            result[indexDst++] = ig;
            result[indexDst++] = ib;
        }
    }

    return result;
}

void Renderer::OneTimeRender()
{
    UpdateTexture(1);
    DrawImage();

    // FBO
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureHandle, 0);

    float* pixels = (float *)malloc(_width*_height* 4 * sizeof(float));
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, _width, _height, GL_RGBA, GL_FLOAT, pixels);

    std::cout << pixels[0] << " " << pixels[1] << " " << pixels[2] << " " << pixels[3] << '\n';

    uint8_t* result = FloatToUint8(pixels);

    stbi_flip_vertically_on_write(true);
    stbi_write_png("test.png",_width, _height, 3, result, _width * 3);
}

Renderer::~Renderer()
{

    delete _renderProgram;
    delete _computeProgram;

}
