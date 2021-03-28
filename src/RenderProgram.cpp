#include <RenderProgram.h>

RenderProgram::RenderProgram(const char* vertexPath, const char* fragmentPath): Program()
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(std::ifstream::failure e)
    {
        throw std::runtime_error("Could not successfuly read shader files");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Compile Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);
    
    int successVS;
    char infoLogVS[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVS);

    if(!successVS)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLogVS);
        throw std::runtime_error("Could not compile vertex shader:\n" + std::string(infoLogVS));
    }

    // compile Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    int successFS;
    char infoLogFS[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFS);

    if(!successFS)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLogFS);
        throw std::runtime_error("Could not compile fragment shader:\n" + std::string(infoLogFS));
    }

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glBindFragDataLocation(id, 0, "color");
    
    glLinkProgram(id);

    // checking if linking successful
    int successSP;
    char infoLogSP[512];
    glGetProgramiv(id, GL_LINK_STATUS, &successSP);
    if(!successSP)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLogSP);
        throw std::runtime_error("Could not link shader program:\n" + std::string(infoLogSP));
    }

    // delete shaders because we don't need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // use the program
    glUseProgram(id);
    glUniform1i(glGetUniformLocation(id, "srcTex"), 0);

    GLuint vertArray;
    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);

    GLuint posBuf;
    glGenBuffers(1, &posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    float data[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, data, GL_STREAM_DRAW);
    GLint posPtr = glGetAttribLocation(id, "pos");
    glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posPtr);

}

RenderProgram::~RenderProgram()
{

}

