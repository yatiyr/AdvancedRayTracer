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
                                    
        }

        aabb.boxmin = glm::vec3(aabb.xmin, aabb.ymin, aabb.zmin);
        aabb.boxmax = glm::vec3(aabb.xmax, aabb.ymax, aabb.zmax);
        return aabb;
        
    }

    static float giveAABBVolume(AABB aabb)
    {
        return (aabb.xmax - aabb.xmin) *
               (aabb.ymax - aabb.ymin) *
               (aabb.zmax - aabb.zmin);
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

    static SplittedIndices splitEqual(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData, int axis)
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

    static SplittedIndices splitMidpoint(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData, int axis)
    {

        SplittedIndices splittedResult;        
        AABB box = computeAABB(indices, vertexData);

        float xlength = box.xmax - box.xmin;
        float ylength = box.ymax - box.ymin;
        float zlength = box.zmax - box.zmin;

        glm::vec3 totalVec(0.0, 0.0, 0.0);
        std::vector<VertexIndex> vivec;

        for(int i=0; i<indices.size(); i++)
        {
            glm::vec3 v = giveTriangleCenter(indices[i], vertexData);
            totalVec += v;

            VertexIndex vi;
            vi.vertex = v;
            vi.index  = i;
            vivec.push_back(vi);

        }

        totalVec /= indices.size();

        if(xlength >= ylength && xlength >= zlength)
        {
            for(int i=0; i<vivec.size(); i++)
            {
                if(vivec[i].vertex.x <= totalVec.x)
                {
                    splittedResult.p1.push_back(indices[vivec[i].index]);
                }
                else
                {
                    splittedResult.p2.push_back(indices[vivec[i].index]);
                }
            }
        }
        else if(ylength >= xlength && ylength >= zlength)
        {
            for(int i=0; i<vivec.size(); i++)
            {
                if(vivec[i].vertex.y <= totalVec.y)
                {
                    splittedResult.p1.push_back(indices[vivec[i].index]);
                }
                else
                {
                    splittedResult.p2.push_back(indices[vivec[i].index]);
                }
            }
        }
        else if(zlength >= ylength && zlength >= xlength)
        {
            for(int i=0; i<vivec.size(); i++)
            {
                if(vivec[i].vertex.z <= totalVec.z)
                {
                    splittedResult.p1.push_back(indices[vivec[i].index]);
                }
                else
                {
                    splittedResult.p2.push_back(indices[vivec[i].index]);
                }
            }
        }

        return splittedResult;
    }

    static SplittedIndices splitSAH(const std::vector<Indices>& indices, const std::vector<Vertex>& vertexData, int axis)
    {
        SplittedIndices splittedResult;

        std::vector<VertexIndex> vivec;

        float medianVal = 0;

        std::array<float, 8> costs;

        float bestBucket = 0;
        int bestSplitAxis = 0;
        float bestCost = FLT_MAX;

        for(size_t i=0; i<indices.size(); i++)
        {
            glm::vec3 v = giveTriangleCenter(indices[i], vertexData);

            VertexIndex vi;
            vi.vertex = v;
            vi.index  = i;
            vivec.push_back(vi);

        }


        std::sort(vivec.begin(), vivec.end(), CompareX());
        float xrange = vivec[vivec.size() - 1].vertex.x - vivec[0].vertex.x;
        float bucketSize = xrange/8;

        for(int i=0; i<7; i++)
        {
            float val = vivec[0].vertex.x + bucketSize*(i+1);

            std::vector<Indices> p1;
            std::vector<Indices> p2;

            for(int j=0; j<vivec.size(); j++)
            {
                if(vivec[j].vertex.x <= val)
                {
                    p1.push_back(indices[vivec[j].index]);
                }
                else
                {
                    p2.push_back(indices[vivec[j].index]);
                }
            }

            if(!p1.empty() && !p2.empty())
            {
                float cost = giveAABBVolume(computeAABB(p1, vertexData)) + giveAABBVolume(computeAABB(p2, vertexData));

                if(cost < bestCost)
                {
                    bestCost = cost;
                    bestSplitAxis = 0;
                    bestBucket = val;
                 }
            }

        }
        

        std::sort(vivec.begin(), vivec.end(), CompareY());
        float yrange = vivec[vivec.size() - 1].vertex.y - vivec[0].vertex.y;
        bucketSize = yrange/8;

        for(int i=0; i<7; i++)
        {
            float val = vivec[0].vertex.y + bucketSize*(i+1);

            std::vector<Indices> p1;
            std::vector<Indices> p2;

            for(int j=0; j<vivec.size(); j++)
            {
                if(vivec[j].vertex.y <= val)
                {
                    p1.push_back(indices[vivec[j].index]);
                }
                else
                {
                    p2.push_back(indices[vivec[j].index]);
                }
            }

            if(!p1.empty() && !p2.empty())
            {
                float cost = giveAABBVolume(computeAABB(p1, vertexData)) + giveAABBVolume(computeAABB(p2, vertexData));

                if(cost < bestCost)
                {
                    bestCost = cost;
                    bestSplitAxis = 1;
                    bestBucket = val;
                }
            }

        }            


        std::sort(vivec.begin(), vivec.end(), CompareZ());
        float zrange = vivec[vivec.size() - 1].vertex.z - vivec[0].vertex.z;
        bucketSize = zrange/8;

        for(int i=0; i<7; i++)
        {
            float val = vivec[0].vertex.z + bucketSize*(i+1);

            std::vector<Indices> p1;
            std::vector<Indices> p2;

            for(int j=0; j<vivec.size(); j++)
            {
                if(vivec[j].vertex.z <= val)
                {
                    p1.push_back(indices[vivec[j].index]);
                }
                else
                {
                    p2.push_back(indices[vivec[j].index]);
                }
            }

            if(!p1.empty() && !p2.empty())
            {
                float cost = giveAABBVolume(computeAABB(p1, vertexData)) + giveAABBVolume(computeAABB(p2, vertexData));

                if(cost < bestCost)
                {
                    bestCost = cost;
                    bestSplitAxis = 2;
                    bestBucket = val;
                }
            }

        }             
        


        if(bestCost != FLT_MAX)
        {
            if(bestSplitAxis == 0)
                std::sort(vivec.begin(), vivec.end(), CompareX());
            else if(bestSplitAxis == 1)
                std::sort(vivec.begin(), vivec.end(), CompareY());
            else if(bestSplitAxis == 2)
                std::sort(vivec.begin(), vivec.end(), CompareZ());

            for(int i=0; i<vivec.size(); i++)
            {
                if(bestSplitAxis == 0)
                {
                    if(vivec[i].vertex.x <= bestBucket)
                    {
                        splittedResult.p1.push_back(indices[vivec[i].index]);
                    }
                    else
                    {
                        splittedResult.p2.push_back(indices[vivec[i].index]);
                    }
                }
                else if(bestSplitAxis == 1)
                {
                    if(vivec[i].vertex.y <= bestBucket)
                    {
                        splittedResult.p1.push_back(indices[vivec[i].index]);
                    }
                    else
                    {
                        splittedResult.p2.push_back(indices[vivec[i].index]);
                    }
                }
                else if(bestSplitAxis == 2)
                {
                    if(vivec[i].vertex.z <= bestBucket)
                    {
                        splittedResult.p1.push_back(indices[vivec[i].index]);
                    }
                    else
                    {
                        splittedResult.p2.push_back(indices[vivec[i].index]);
                    }
                }
            }
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
        if(depth == maxDepth || indices.size() <= 8)
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

        SplittedIndices si = splitMidpoint(indices, vertexData, axis);
        
        node.indicesSize = 0;
        node.indicesOffset = 0;
        BVHNodes.push_back(node);
        int vecsize = BVHNodes.size();
        nodeCounter++;

        if(si.p1.empty() || si.p2.empty())
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
            indexCounter += indices.size();

            return 1;
        }

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