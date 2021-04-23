#ifndef __SCENE_H__
#define __SCENE_H__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include <tinyxml2.h>
#include <sstream>
#include <ComputeProgram.h>


#include <RootDir.h>


#include <Utils.h>

#include <list>


#define MAX_DEPTH 100


class Scene
{
private:
 

    

public:

    GLuint ssbo_pointLights;
    GLuint ssbo_materials;
    GLuint ssbo_vertexData;
    GLuint ssbo_meshes;
    GLuint ssbo_triangles;
    GLuint ssbo_spheres;
    GLuint ssbo_cameras;
      
    GLuint ssbo_BVHNormals;
    GLuint ssbo_BVHIndices;
    GLuint ssbo_BVHNodes;

    std::vector<PointLight> _pointLights;
    std::vector<Material>   _materials;
    std::vector<Vertex>     _vertexData;
    std::vector<Mesh>       _meshes;
    std::vector<Triangle>   _triangles;
    std::vector<Sphere>     _spheres;
    std::vector<Camera>    _cameras;

    std::vector<Indices> _BVHIndices;
    std::vector<BVHNode> _BVHNodes;
    std::vector<Vertex> _BVHNormals;


    glm::vec3               _backgroundColor;
    glm::vec3               _ambientLight;

    Camera                 _activeCamera;
    std::vector<std::string> imageNames;

    float _shadowRayEpsilon;
    float _intersectionTestEpsilon;

    std::string _imageName;

    int _maxRecursionDepth;
    
    void BindObjectsToGPU();

    void SetUniforms(ComputeProgram* program);

    void ComputeFaceNormals();

    Scene(const std::string& filepath);
    ~Scene();

};


#endif /* __SCENE_H__ */