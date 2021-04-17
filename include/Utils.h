#ifndef __UTILS_H__
#define __UTILS_H__

#include <Structures.h>


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

inline VertexIndex giveTriangleCenter(const Indices& face, const std::vector<Vertex>& vertexData, int index)
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

    SplittedIndices splittedResult;

    std::vector<VertexIndex> vec;

    for(size_t i=0; i<indices.size(); i++)
    {
        VertexIndex vi = giveTriangleCenter(indices[i], vertexData, i);
        vec.push_back(vi);
    }

    if(axis == 0)
    {
        std::sort(vec.begin(), vec.end(), CompareX());
    }
    else if(axis == 1)
    {
        std::sort(vec.begin(), vec.end(), CompareY());
    }
    else if(axis ==2)
    {
        std::sort(vec.begin(), vec.end(), CompareZ());
    }

    for(size_t i=0; i<vec.size()/2; i++)
    {
        splittedResult.p1.push_back(indices[vec[i].index]);
    }

    for(size_t i=vec.size()/2; i<vec.size(); i++)
    {
        splittedResult.p2.push_back(indices[vec[i].index]);
    }

    return splittedResult;

}

inline int constructBVH(const std::vector<Indices>& indices,
                        const std::vector<Vertex>& vertexData,
                        std::vector<Indices>& BVHIndices,
                        std::vector<BVHNode>& BVHNodes,
                        int& nodeCounter,
                        int& indexCounter,
                        int depth,
                        int maxDepth,
                        int axis)
{
    BVHNode node;
    node.aabb = computeAABB(indices, vertexData);
    if(depth == maxDepth || indices.size() == 1)
    {
        for(size_t i=0; i<indices.size(); i++)
        {
            BVHIndices.push_back(indices[i]);
        }

        node.indicesOffset = indexCounter;
        node.indicesSize   = indices.size();
        node.leftNode = -1;
        node.rightNode = -1;
        BVHNodes.push_back(node);
        nodeCounter++;
        indexCounter += indices.size();

        return (nodeCounter - 1);
    }

    SplittedIndices si = split(indices, vertexData, axis);
    node.leftNode      = constructBVH(si.p1,
                                      vertexData,
                                      BVHIndices,
                                      BVHNodes,
                                      nodeCounter,
                                      indexCounter,
                                      depth + 1,
                                      maxDepth,
                                      (axis + 1)%3);

    node.rightNode      = constructBVH(si.p2,
                                      vertexData,
                                      BVHIndices,
                                      BVHNodes,
                                      nodeCounter,
                                      indexCounter,
                                      depth+1,
                                      maxDepth,
                                      (axis + 1)%3);

    node.indicesSize = 0;
    node.indicesOffset = 0;
    BVHNodes.push_back(node);
    nodeCounter++;
    return (nodeCounter - 1);

}

#endif /* __UTILS_H__ */