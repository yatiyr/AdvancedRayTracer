#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stb_image.h>
#include <RootDir.h>

#include <ComputeProgram.h>
#include <RenderProgram.h>

#include <stb_image.h>
#include <stb_image_write.h>
#include <Timer.h>

#include <SceneManager.h>
#include <vector>

#include <thread>
#include <CPURaytracer.h>




enum RenderingMode
{
    GPU_RENDERING_BIT = 0x00000,
    CPU_RENDERING_BIT = 0x00001,
    OFFSCREEN_RENDERING_BIT = 0x00002,
};

struct WorkGroup
{
    int start;
    int end;
};

// Base renderer class
class Renderer
{
private:


    float* _pixels;
    Scene* _scenePtr;
    std::vector<WorkGroup> _workGroups;

    // 
    RenderingMode renderMode;

    // This objects keeps track of scenes we add
    SceneManager _sceneManager;

    // store glfw window pointer here
    GLFWwindow* _window;

    ComputeProgram* _computeProgram;
    RenderProgram* _renderProgram;

    // These are needed for measuring
    // delta time for interpolations etc.
    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;

    // windows size
    unsigned int _width;
    unsigned int _height;

    // store the texture we are going to write for ray tracing
    GLuint _textureHandle;

    // used for gl's framebuffer size callbacks
    static void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);

    // used for gl's mouse position callbacks
    static void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);

    // used for gl's scroll callbacks
    static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

    // process other inputs
    static void Process_Input(GLFWwindow* window);

    // load opengl function pointers
    void LoadFunctionPointers();

    static void PrintDeviceProperties();

    static void FindComputeShaderExtension();

    void GenerateTexture();

    void UpdateTexture(float frame);

    void DrawImage();

    void InitializeShaderPrograms();


    void GetMaximumWorkGroupCount();

    void GetMaximumWorkGroupSize();

    static void WritePixelCoord(float* pixels, int i, int j, glm::vec3, int width);

    uint8_t* FloatToUint8(float* pixels, int width, int height);

    static glm::vec2 GiveCoords(int index, int height, int width);

    static void ProcessWorkGroup(float* pixels, WorkGroup wg, int height, int width, Scene* scenePtr);

    void ClearImage();



public:

    // Initial width and height are 800 and 600 by default
    Renderer();

    Renderer(const char* file, RenderingMode mode);

    ~Renderer();

    void RenderLoop();

    void OneTimeRender();

    void CPURender();

    static void InitializeGL(int height, int width, RenderingMode mode);

};




#endif /* __RENDERER_H__ */