#ifndef __UTILS_H__
#define __UTILS_H__

#include <Structures.h>

class Utils
{
public:
    static AABB computeAABB(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData)
    {

        AABB aabb;
        aabb.xmin = FLT_MAX;
        aabb.ymin = FLT_MAX;
        aabb.zmin = FLT_MAX;

        aabb.xmax = -FLT_MAX;
        aabb.ymax = -FLT_MAX;
        aabb.zmax = -FLT_MAX;

        std::multiset<float> x;
        std::multiset<float> y;
        std::multiset<float> z;

        
        for(int i = 0; i<indices.size(); i++)
        {
            
            float ax = vertexData[indices[i].a -1].pos.x;
            float bx = vertexData[indices[i].b -1].pos.x;
            float cx = vertexData[indices[i].c -1].pos.x;

            float ay = vertexData[indices[i].a -1].pos.y;
            float by = vertexData[indices[i].b -1].pos.y;
            float cy = vertexData[indices[i].c -1].pos.y;

            float az = vertexData[indices[i].a -1].pos.z;
            float bz = vertexData[indices[i].b -1].pos.z;
            float cz = vertexData[indices[i].c -1].pos.z;


            if(ax < aabb.xmin)
            {
                aabb.xmin = ax;
            }
            if(ax > aabb.xmax)
            {
                aabb.xmax = ax;
            }

            if(bx < aabb.xmin)
            {
                aabb.xmin = bx;
            }
            if(bx > aabb.xmax)
            {
                aabb.xmax = bx;
            }

            if(cx < aabb.xmin)
            {
                aabb.xmin = cx;
            }
            if(cx > aabb.xmax)
            {
                aabb.xmax = cx;
            }

        // --------------------------- //


            if(ay < aabb.ymin)
            {
                aabb.ymin = ay;
            }
            if(ay > aabb.ymax)
            {
                aabb.ymax = ay;
            }

            if(by < aabb.ymin)
            {
                aabb.ymin = by;
            }
            if(by > aabb.ymax)
            {
                aabb.ymax = by;
            }

            if(cy < aabb.ymin)
            {
                aabb.ymin = cy;
            }
            if(cy > aabb.ymax)
            {
                aabb.ymax = cy;
            }

        // ------------------------ //


            if(az < aabb.zmin)
            {
                aabb.zmin = az;
            }
            if(az > aabb.zmax)
            {
                aabb.zmax = az;
            }

            if(bz < aabb.zmin)
            {
                aabb.zmin = bz;
            }
            if(bz > aabb.zmax)
            {
                aabb.zmax = bz;
            }

            if(cz < aabb.zmin)
            {
                aabb.zmin = cz;
            }
            if(cz > aabb.zmax)
            {
                aabb.zmax = cz;
            }                
        
            
    
/*
            x.insert(vertexData[indices[i].a -1].pos.x);
            x.insert(vertexData[indices[i].b -1].pos.x);
            x.insert(vertexData[indices[i].c -1].pos.x);

            y.insert(vertexData[indices[i].a -1].pos.y);
            y.insert(vertexData[indices[i].b -1].pos.y);
            y.insert(vertexData[indices[i].c -1].pos.y);

            z.insert(vertexData[indices[i].a -1].pos.z);
            z.insert(vertexData[indices[i].b -1].pos.z);
            z.insert(vertexData[indices[i].c -1].pos.z);  

    */
                                    
        }

        /*
        aabb.xmin = *(x.begin());
        aabb.ymin = *(y.begin());
        aabb.zmin = *(z.begin()); 

        aabb.xmax = *(x.rbegin());
        aabb.ymax = *(y.rbegin());
        aabb.zmax = *(z.rbegin());     
    */
        return aabb;
        
    }

    static glm::vec3 giveTriangleCenter(Indices face, const std::vector<Vertex>& vertexData)
    {
        glm::vec3 result;
        result  = (vertexData[face.a - 1].pos + vertexData[face.b -1].pos + vertexData[face.c -1].pos);
        result.x /= 3;
        result.y /= 3;
        result.z /= 3;

        return result;
    }

    static SplittedIndices split(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData, int axis)
    {

        SplittedIndices splittedResult;

        std::vector<VertexIndex> vivec;

        float medianVal = 0;

        for(size_t i=0; i<indices.size(); i++)
        {
            glm::vec3 v = giveTriangleCenter(indices[i], vertexData);

            VertexIndex vi;
            vi.vertex = v;
            vi.index  = i;
            vivec.push_back(vi);

        }

        if(axis == 0)
        {
            std::sort(vivec.begin(), vivec.end(), CompareX());
        }
        else if(axis == 1)
        {
            std::sort(vivec.begin(), vivec.end(), CompareY());
        }
        else if(axis == 2)
        {
            std::sort(vivec.begin(), vivec.end(), CompareZ());
        }

        
    
        for(size_t i=0; i<vivec.size()/2; i++)
        {
            splittedResult.p1.push_back(indices[vivec[i].index]);
        }

        for(size_t i=vivec.size()/2; i<vivec.size(); i++)
        {
            splittedResult.p2.push_back(indices[vivec[i].index]);
        }
        

        return splittedResult;

    }

    static int constructBVH(const std::vector<Indices>& indices,
                            const std::vector<Vertex>& vertexData,
                            std::vector<Indices>& BVHIndices,
                            std::vector<BVHNode>& BVHNodes,
                            int& indexCounter,
                            int& nodeCounter,
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
            node.childSize = 1;
            BVHNodes.push_back(node);
            nodeCounter++;
            indexCounter += indices.size();

            return 1;
        }

        SplittedIndices si = split(indices, vertexData, axis);
        
        node.indicesSize = 0;
        node.indicesOffset = 0;
        BVHNodes.push_back(node);
        int vecsize = BVHNodes.size();
        nodeCounter++;

        int totalNode = 1 + constructBVH(si.p1, vertexData, BVHIndices, BVHNodes,
                            indexCounter, nodeCounter, depth + 1, maxDepth, (axis + 1)%3)
                            +
                            constructBVH(si.p2, vertexData, BVHIndices, BVHNodes,
                            indexCounter, nodeCounter, depth + 1, maxDepth, (axis + 1)%3);

        BVHNodes[vecsize - 1].childSize = totalNode;
                                      
        return totalNode;

    }
};

#endif /* __UTILS_H__ */