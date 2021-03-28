#include <Program.h>

Program::Program()
{

}

Program::~Program()
{

}

void Program::Use()
{
    glUseProgram(id);
}

void Program::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Program::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Program::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Program::SetVec3(const std::string& name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Program::SetVec3(const std::string& name, glm::vec3 vec)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
}

void Program::SetVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(id,name.c_str()), x, y, z, w);
}

void Program::SetVec4(const std::string& name, glm::vec4 vec)
{
    glUniform4f(glGetUniformLocation(id,name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Program::Set4Matrix(const std::string& name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::Set3Matrix(const std::string& name, glm::mat3 matrix)
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}