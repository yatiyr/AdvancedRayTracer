#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <glm/glm.hpp>
#include <set>
#include <vector>
#include <algorithm>

struct Camera
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 gaze;
    alignas(16) glm::vec3 up;
    alignas(16) glm::vec3 v;
    alignas(16) glm::vec4 nearPlane;
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
    int rootBVHNode;
};


struct Sphere
{
    int centerVertexId;
    int materialId;
    float radius;
};


struct VertexIndex
{
    alignas(16) glm::vec3 vertex;
    int index;
};

// Comparator functions
struct CompareX
{
    inline bool operator() (const VertexIndex& lhs, const VertexIndex& rhs) const
    {
        return lhs.vertex.x <= rhs.vertex.x;
    }
};

struct CompareY
{
    inline bool operator() (const VertexIndex& lhs, const VertexIndex& rhs) const
    {
        return lhs.vertex.y <= rhs.vertex.y;
    }
};

struct CompareZ
{
    inline bool operator() (const VertexIndex& lhs, const VertexIndex& rhs) const
    {
        return lhs.vertex.z <= rhs.vertex.z;
    }
};

struct SplittedIndices
{
    std::vector<Indices> p1;
    std::vector<Indices> p2;
};


struct AABB
{
    float xmax;
    float ymax;
    float zmax;
    float xmin;
    float ymin;
    float zmin;
};

struct BVHNode
{
    AABB aabb;
    size_t leftNode;
    size_t rightNode;
    size_t indicesOffset;
    size_t indicesSize;
};



#endif