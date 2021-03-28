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




// Base renderer class
class Renderer
{
private:

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




public:

    // Initial width and height are 800 and 600 by default
    Renderer(unsigned int initialWidth = 800, unsigned int initialHeight = 600);

    ~Renderer();

    void RenderLoop();

    static void InitializeGL(int height, int width);

};




#endif /* __RENDERER_H__ */