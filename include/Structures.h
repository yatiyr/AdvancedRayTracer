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
    bool operator() (const VertexIndex& lhs, const VertexIndex& rhs) const
    {
        return lhs.vertex.x <= rhs.vertex.x;
    }
};

struct CompareY
{
    bool operator() (const VertexIndex& lhs, const VertexIndex& rhs) const
    {
        return lhs.vertex.y <= rhs.vertex.y;
    }
};

struct CompareZ
{
    bool operator() (const VertexIndex& lhs, const VertexIndex& rhs) const
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

};



inline AABB computeAABB(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData)
{

    AABB aabb;

    std::multiset<float> x;
    std::multiset<float> y;
    std::multiset<float> z;

    for(int i = 0; i<indices.size(); i++)
    {
        x.insert(vertexData[indices[i].a -1].pos.x);
        x.insert(vertexData[indices[i].b -1].pos.x);
        x.insert(vertexData[indices[i].c -1].pos.x);

        y.insert(vertexData[indices[i].a -1].pos.y);
        y.insert(vertexData[indices[i].b -1].pos.y);
        y.insert(vertexData[indices[i].c -1].pos.y);

        z.insert(vertexData[indices[i].a -1].pos.z);
        z.insert(vertexData[indices[i].b -1].pos.z);
        z.insert(vertexData[indices[i].c -1].pos.z);                             
    }

    aabb.xmin = *(x.begin());
    aabb.ymin = *(y.begin());
    aabb.zmin = *(y.begin()); 

    aabb.xmax = *(x.rbegin());
    aabb.ymax = *(y.rbegin());
    aabb.zmax = *(y.rbegin());

    return aabb;
    
}

inline VertexIndex giveTriangleCenter(const Indices& face, int index, const std::vector<Vertex>& vertexData)
{
    VertexIndex result;
    result.vertex = (vertexData[face.a - 1].pos + vertexData[face.b -1].pos + vertexData[face.c -1].pos);
    result.vertex.x /= 3;
    result.vertex.y /= 3;
    result.vertex.z /= 3;

    result.index = index;

    return result;
}

inline SplittedIndices split(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData, int axis)
{

    if(axis == 0)
    {
        
    }
    else if(axis == 1)
    {

    }
    else if(axis ==2)
    {

    }

}




#endif