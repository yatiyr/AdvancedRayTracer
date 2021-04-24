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

void Renderer::InitializeGL(int width, int height, RenderingMode mode)
{
    glfwInit();

    // Use OpenGL 4.6 as contest
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    if(mode & RenderingMode::OFFSCREEN_RENDERING_BIT)
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

}


// For now this constructor will be used for demonstration
Renderer::Renderer(const char* file, RenderingMode mode)
{
    if(mode & RenderingMode::CPU_RENDERING_BIT)
    {
        _sceneManager.AddScene(std::string(ROOT_DIR) + std::string(file));
    }
    else
    {
        _sceneManager.AddScene(std::string(ROOT_DIR) + std::string(file));

        _width = _sceneManager.GetScene(0)._activeCamera.imageResolution.x;
        _height = _sceneManager.GetScene(0)._activeCamera.imageResolution.y;

        InitializeGL(_width, _height, mode);

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


        glViewport(0, 0, _width, _height);

        // read load compile and link shader programs
        InitializeShaderPrograms();


        //PrintDeviceProperties();
        //FindComputeShaderExtension();
        //GetMaximumWorkGroupCount();
        //GetMaximumWorkGroupSize();
    }

}

Renderer::Renderer()
{

    _sceneManager.AddScene(std::string(ROOT_DIR) + "assets/scenes/two_spheres.xml");


    _width  = _sceneManager.GetScene(0)._activeCamera.imageResolution.x;
    _height = _sceneManager.GetScene(0)._activeCamera.imageResolution.y;

    InitializeGL(_width, _height, RenderingMode::GPU_RENDERING_BIT);

    
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


    glViewport(0, 0, _width, _height);

    // read load compile and link shader programs
    InitializeShaderPrograms();


    //PrintDeviceProperties();
    //FindComputeShaderExtension();
    //GetMaximumWorkGroupCount();
    //GetMaximumWorkGroupSize();

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
    
    _sceneManager.LoadScene(0,_computeProgram);

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

uint8_t* Renderer::FloatToUint8(float* pixels, int width, int height)
{
    // we will omit alpha channel
    uint8_t* res = new uint8_t[width * height * 3];

    int indexSrc = 0;
    int indexDst = 0;
    for(int j= 0; j<width; j++)
    {
        for(int i=0; i<height; i++)
        {
            float r = pixels[indexSrc++];
            float g = pixels[indexSrc++];
            float b = pixels[indexSrc++];
            indexSrc++;

            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            res[indexDst++] = ir;
            res[indexDst++] = ig;
            res[indexDst++] = ib;
        }
    }

    return res;
}

void Renderer::OneTimeRender()
{
    float * pixels;
    uint8_t*  result;

    _sceneManager.LoadScene(0,_computeProgram);

    {
        Timer t;
        UpdateTexture(1);        
        DrawImage();        
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureHandle, 0);             

        std::cout << "Image rendered in --->";
    }
        glReadBuffer(GL_COLOR_ATTACHMENT0);   
        pixels = (float *)malloc(_width*_height* 4 * sizeof(float));        
        glReadPixels(0, 0, _width, _height, GL_RGBA, GL_FLOAT, pixels); 

        result = FloatToUint8(pixels, _width, _height);

        stbi_flip_vertically_on_write(true);
        std::string outputPath = "outputs/" + _sceneManager.GetScene(0)._imageName;
        stbi_write_png(outputPath.c_str(),_width, _height, 3, result, _width * 3);      

        // I was using it for debugging compute shader
        std::cout << pixels[0] << " " << pixels[1] << " " << pixels[2] << " " << pixels[3] << '\n';

        free(pixels);
        delete[] result;
    
}

void Renderer::WritePixelCoord(float* pixels, int i, int j, glm::vec3 color, int width)
{
    pixels[j* 4 + (width * i *4)] = color.x;
    pixels[j * 4 + (width * i *4) + 1] = color.y;
    pixels[j * 4 + (width * i *4) + 2] = color.z;
    pixels[j * 4 + (width * i *4) + 3] = 1.0f;
}

void Renderer::ClearImage()
{
    for(size_t i=0; i<_height; i++)
    {
        for(size_t j=0; j<_width; j++)
        {
            _pixels[j* 4 + (_width * i *4)] = 0.0;
            _pixels[j * 4 + (_width * i *4) + 1] = 0.0;
            _pixels[j * 4 + (_width * i *4) + 2] = 0.0;
            _pixels[j * 4 + (_width * i *4) + 3] = 1.0f;            
        }
    }
}


glm::vec2 Renderer::GiveCoords(int index, int height, int width)
{
    glm::vec2 result;

    int i = index/width;
    int j = index%width;

    result.x = i;
    result.y = j;

    return result;
}

void Renderer::ProcessWorkGroup(float* pixels, WorkGroup wg, int height, int width, Scene* scenePtr)
{
    for(int i=wg.start; i<=wg.end; i++)
    {
        glm::vec2 coords = GiveCoords(i, height, width);
        Ray pR = ComputePrimaryRay(coords.x, coords.y, scenePtr->_cameras[0]);
        glm::vec3 pixel = RayTrace(pR, scenePtr);
        WritePixelCoord(pixels, coords.x, coords.y, pixel, width);
    }
}

void Renderer::CPURender()
{
    uint8_t *result;

    _sceneManager.LoadSceneForCPU(0);

    _scenePtr = _sceneManager.GetScenePointer(0);

    _width = _scenePtr->_cameras[0].imageResolution.x;
    _height = _scenePtr->_cameras[0].imageResolution.y;

    _pixels = (float *)malloc(_width*_height*4*sizeof(float));
    ClearImage();

    int totalSize = _width*_height;
    int workGroupSize = 32;

    int chunkSize = totalSize/workGroupSize;

    for(int i=0; i<workGroupSize; i++)
    {
        int start;
        int end;

        start = i * chunkSize;
        if(i == workGroupSize - 1)
            end = (i+1) * chunkSize - 1 + (totalSize%chunkSize);
        else
            end = (i+1) * chunkSize - 1;

        WorkGroup wg;
        wg.start = start;
        wg.end   = end;

        _workGroups.push_back(wg);

        if(i != 3 && i != 6)
        {
            //std::thread t(ProcessWorkGroup, _pixels, wg, _height, _width);
            //t.join();
        }
    }

    {
        Timer t;
        for(int i=0; i<workGroupSize; i++)
        {
            std::thread t(ProcessWorkGroup, _pixels, _workGroups[i], _height, _width, _scenePtr);
            t.join();
        }
        std::cout << "Image rendered in --->";
    }

    //ProcessWorkGroup(_workGroups[workGroupSize-1]);

    result = FloatToUint8(_pixels, _width, _height);

    //stbi_flip_vertically_on_write(true);
    std::string outputPath = "outputs/cpu/" + _scenePtr->_imageName;
    stbi_write_png(outputPath.c_str(),_width, _height, 3, result, _width * 3);     

    free(_pixels);
    delete[] result;

}

Renderer::~Renderer()
{

    //delete _renderProgram;
    //delete _computeProgram;

}
