#include <ComputeProgram.h>

ComputeProgram::ComputeProgram(const char* computePath)
{
    std::string computeShaderCode;
    std::ifstream cShaderFile;

    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


    try
    {
        cShaderFile.open(computePath);

        std::stringstream cShaderStream;

        cShaderStream << cShaderFile.rdbuf();

        cShaderFile.close();

        computeShaderCode   = cShaderStream.str();

    }
    catch(std::ifstream::failure e)
    {
        throw std::runtime_error("Could not successfuly read compute shader file");
    }

    const char* cShaderCode = computeShaderCode.c_str();

    int success;
    char infoLog[512];

    // Compile Compute Shader
    unsigned int computeShader;
    computeShader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(computeShader, 1, &cShaderCode, nullptr);
    glCompileShader(computeShader);
    
    int successCS;
    char infoLogVS[512];
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &successCS);

    if(!successCS)
    {
        glGetShaderInfoLog(computeShader, 512, nullptr, infoLogVS);
        throw std::runtime_error("Could not compile compute shader:\n" + std::string(infoLogVS));
    }

    id = glCreateProgram();
    glAttachShader(id, computeShader);
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

    // delete compute shader because we don't need it
    glDeleteShader(computeShader);

    // use the program
    glUseProgram(id);

    glUniform1i(glGetUniformLocation(id, "imgOutput"), 0);

}

ComputeProgram::~ComputeProgram()
{

}
