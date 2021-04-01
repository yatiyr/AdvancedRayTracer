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
#include <Program.h>

#include <RootDir.h>

struct Camera
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 gaze;
    alignas(16) glm::vec3 up;
    glm::vec4 nearPlane;
    glm::vec2 imageResolution;       
    float nearDistance;
};

struct PointLight
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 intensity;
};

struct Material
{
    alignas(16) glm::vec3 ambientReflectance;
    alignas(16) glm::vec3 diffuseReflectance;
    alignas(16) glm::vec3 specularReflectance;
    alignas(16) glm::vec3 mirrorReflectance;
    float phongExponent;
};

struct Vertex
{
    alignas(16) glm::vec3 pos;
};

struct Indices
{
    int a;
    int b;
    int c;
};

struct Triangle
{
    int materialId;
    Indices indices;
};


struct Mesh
{
    int materialId;
    int indicesOffset;
    int indicesSize;
};


struct Sphere
{
    int centerVertexId;
    int materialId;
    float radius;
};

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
    GLuint ssbo_meshIndexBuffer;
    GLuint ssbo_cameras;
    GLuint ssbo_meshIndices;
      

    std::vector<PointLight> _pointLights;
    std::vector<Material>   _materials;
    std::vector<Vertex>     _vertexData;
    std::vector<Mesh>       _meshes;
    std::vector<Triangle>   _triangles;
    std::vector<Sphere>     _spheres;
    std::vector<Camera>    _cameras;

    // all indices of all meshes in the scene are
    // recorded in this buffer and sent to gpu as one buffer
    // in meshes, we will have offsets and sizes for picking up
    // their corresponding indices
    std::vector<Indices>    _meshIndexBuffer;

    glm::vec3               _backgroundColor;
    glm::vec3               _ambientLight;

    Camera                 _activeCamera;
    std::vector<std::string> imageNames;

    float _shadowRayEpsilon;
    float _intersectionTestEpsilon;

    std::string _imageName;

    int _maxRecursionDepth;
    
    void BindObjectsToGPU();

    void SetUniforms(Program* program);

    Scene(const std::string& filepath);
    ~Scene();

};


#endif /* __SCENE_H__ */