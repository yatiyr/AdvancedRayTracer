#ifndef __SCENE_H__
#define __SCENE_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct activeCamera
{
    glm::vec4 position;
    glm::vec4 gaze;
    glm::vec4 up;
    glm::vec4 nearPlane;
    glm::vec4 nearDistance;
    glm::vec2 imageResolution;
};

struct PointLight
{
    glm::vec4 position;
    glm::vec4 intensity;
};

struct Material
{
    glm::vec4 ambientReflectance;
    glm::vec4 diffuseReflectance;
    glm::vec4 specularReflectance;
    float phongExponent;
};

struct Vertex
{
    glm::vec4 pos;
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
    std::vector<Indices> faces;
};


struct Sphere
{
    glm::vec4 centerPos;
    int materialId;
    float radius;
};

class Scene
{
private:

    glm::vec4               _backgroundColor;
    glm::vec4               _ambientLight;
    activeCamera            _camera;
    std::vector<PointLight> _pointLights;
    std::vector<Material>   _materials;
    std::vector<Vertex>     _vertexData;
    std::vector<Mesh>       _meshes;
    std::vector<Triangle>   _triangles;
    std::vector<Sphere>     _spheres;

    float _shadowRayEpsilon;
    float _intersectionTestEpsilon;
 

    

public:
    Scene();
    ~Scene();

};


#endif /* __SCENE_H__ */