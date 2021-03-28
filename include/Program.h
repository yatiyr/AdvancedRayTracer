#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <iostream>
#include <fstream>
#include <sstream>

#include <RootDir.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include <string>

class Program
{
public:
    GLuint id;

    Program();
    ~Program();

    void Use();

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, float x, float y, float z);
    void SetVec3(const std::string& name, glm::vec3 vec);
    void SetVec4(const std::string& name, float x, float y, float z, float w);
    void SetVec4(const std::string& name, glm::vec4 vec);
    void Set4Matrix(const std::string& name, glm::mat4 matrix);
    void Set3Matrix(const std::string& name, glm::mat3 matrix);    
};

#endif /* __PROGRAM_H__ */