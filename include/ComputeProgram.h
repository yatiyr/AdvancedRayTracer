#ifndef __COMPUTE_PROGRAM_H__
#define __COMPUTE_PROGRAM_H__

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
#include <Program.h>

class ComputeProgram : public Program
{
public:
    GLuint id;

    ComputeProgram(const char* computePath);

    ~ComputeProgram();

};


#endif /* __COMPUTE_PROGRAM_H__ */