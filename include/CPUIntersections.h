#ifndef __CPU_INTERSECTIONS__
#define  __CPU_INTERSECTIONS__

#include <Structures.h>


inline IntersectionReport intersectSphere(Ray r, Sphere s, float tmin, float tmax, const std::vector<Vertex>& vertexData)
{
    IntersectionReport report;
    report.dbg = 0;

    report.intersection = glm::vec3(-FLT_MAX);
    report.d = FLT_MAX;

    glm::vec3 sphereCenter = vertexData[s.centerVertexId - 1].pos;

    float discriminant = pow(glm::dot(r.direction, (r.origin - sphereCenter)), 2) -
                         dot(r.direction, r.direction) * (glm::dot(r.origin - sphereCenter, r.origin - sphereCenter) -
                         s.radius * s.radius);

    float t;

    if(discriminant >= 0)
    {
        float t1 = -(glm::dot(r.direction, (r.origin - sphereCenter)) + sqrt(discriminant)) / glm::dot(r.direction, r.direction);
        float t2 = -(glm::dot(r.direction, (r.origin - sphereCenter)) - sqrt(discriminant)) / glm::dot(r.direction, r.direction);

        t = std::min(t1, t2);

        if(t > tmin && t < tmax)
        {
            report.d = t;
            report.intersection = r.origin + t*r.direction;
            report.materialId   = s.materialId;
            report.normal       = glm::normalize(report.intersection - sphereCenter);
        }

        return report;
    }

    return report;
}

inline bool improvedBoxIntersect(Ray r, AABB box, float tmin, float tmax)
{
    glm::vec3 t0s = (box.boxmin - r.origin) * r.rcp;
    glm::vec3 t1s = (box.boxmax - r.origin) * r.rcp;

    glm::vec3 tsmaller = glm::min(t0s, t1s);
    glm::vec3 tbigger  = glm::max(t0s, t1s);

    float nTmin = std::max(tmin, std::max(tsmaller[0], std::max(tsmaller[1], tsmaller[2])));
    float nTmax = std::min(tmax, std::min(tbigger[0], std::min(tbigger[1], tbigger[2])));

    return (tmin < tmax);
}

inline bool intersectBox(Ray r, AABB box, float tmin, float tmax)
{
    float t1x = glm::clamp((box.xmin - r.origin.x) / r.direction.x, tmin, tmax);
    float t2x = glm::clamp((box.xmax - r.origin.x) / r.direction.x, tmin, tmax);

	if(t1x > t2x)
	{
		float tmp = t1x;
		t1x = t2x;
		t2x = tmp;
	}

    float t1y = glm::clamp((box.ymin - r.origin.x) / r.direction.y, tmin, tmax);
    float t2y = glm::clamp((box.ymax - r.origin.x) / r.direction.y, tmin, tmax);

	if(t1y > t2y)
	{
		float tmp = t1y;
		t1y = t2y;
		t2y = tmp;
	}

	float t1z = glm::clamp((box.zmin - r.origin.z) / r.direction.z, tmin, tmax);
	float t2z = glm::clamp((box.zmax - r.origin.z) / r.direction.z, tmin, tmax); 

	if(t1z > t2z)
	{
		float tmp = t1z;
		t1z = t2z;
		t2z = tmp;
	}

	float t1max = t1x;
	float t2min = t2x;

	if(t1max < t1y)
		t1max = t1y;
	if(t1max < t1z)
		t1max = t1z;

	if(t2min > t2y)
		t2min = t2y;
	if(t2min > t2z)
		t2min = t2z;

	if(t1max > t2min)
	{	
		return false;
	}
	else if(t1max < t2min)
	{
		return true;
	}
	else if(t1max == t2min)
	{
		return true;
	}
	else
	{
		return false;
	}    

}

inline IntersectionReport MollerTrumboreIntersection(Ray r, Indices indices, float tmin, float tmax, std::vector<Vertex> &vertexData, float intersectionTestEpsilon)
{
    IntersectionReport report;
    report.intersection = glm::vec3(-FLT_MAX);
    report.d = FLT_MAX;
    report.dbg = 0;

    glm::vec3 v0 = vertexData[indices.a - 1].pos;
    glm::vec3 v1 = vertexData[indices.b - 1].pos;
    glm::vec3 v2 = vertexData[indices.c - 1].pos;

    glm::vec3 edge1, edge2, h, s, q;
    float a, f, u, v;

    edge1 = v1 - v0;
    edge2 = v2 - v0;

    h = glm::cross(r.direction, edge2);
    a = glm::dot(edge1, h);

    if(a > -intersectionTestEpsilon && a < intersectionTestEpsilon)
        return report;

    f = 1.0 / a;
    s = r.origin - v0;
    u = f * glm::dot(s, h);

    if(u < 0.0 || u > 1.0)
        return report;

    q = glm::cross(s, edge1);
    v = f * glm::dot(r.direction, q);

    if(v < 0.0 || u + v > 1.0)
        return report;

    float t = f * glm::dot(edge2, q);

    if(t > intersectionTestEpsilon && t > tmin && t < tmax)
    {
        report.intersection = r.origin + r.direction * t;
        report.d = t;
    }

    return report;
}


inline IntersectionReport intersectFace(Ray r, Indices indices, float tmin, float tmax, const std::vector<Vertex>& vertexData, float intersectionTestEpsilon)
{
	IntersectionReport report;
	report.intersection = glm::vec3(-FLT_MAX);
	report.d = FLT_MAX;	
	report.dbg = 0;		

	glm::vec3 a = vertexData[indices.a - 1].pos;
	glm::vec3 b = vertexData[indices.b - 1].pos;
	glm::vec3 c = vertexData[indices.c - 1].pos;

	float detA = glm::determinant(glm::mat3(glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z),
	                              glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z),
								  glm::vec3(r.direction.x, r.direction.y, r.direction.z)));


	float beta = glm::determinant(glm::mat3(glm::vec3(a.x - r.origin.x, a.y - r.origin.y, a.z - r.origin.z),
	                              glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z),
								  glm::vec3(r.direction.x, r.direction.y, r.direction.z)))/detA;

	float gamma = glm::determinant(glm::mat3(glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z),
	                               glm::vec3(a.x - r.origin.x, a.y - r.origin.y, a.z - r.origin.z),
								   glm::vec3(r.direction.x, r.direction.y, r.direction.z)))/detA;


	float t = glm::determinant(glm::mat3(glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z),
	                           glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z),
							   glm::vec3(a.x - r.origin.x, a.y - r.origin.y, a.z - r.origin.z)))/detA;


	if((beta + gamma <= 1 + intersectionTestEpsilon) && (beta >= -intersectionTestEpsilon) && (gamma >= -intersectionTestEpsilon) && t>tmin && t<tmax)
	{
		report.d = t;
		report.intersection = r.origin + t*r.direction;		
	}

	return report;    
}

inline IntersectionReport intersectTriangle(Ray r, Triangle triangle, const std::vector<Vertex>& vertexData, float tmin, float tmax, float intersectionTestEpsilon)
{
    IntersectionReport report = intersectFace(r, triangle.indices, tmin, tmax, vertexData, intersectionTestEpsilon);
    report.materialId = triangle.materialId;

	glm::vec3 a = vertexData[triangle.indices.a-1].pos;
	glm::vec3 b = vertexData[triangle.indices.b-1].pos;
	glm::vec3 c = vertexData[triangle.indices.c-1].pos;

	report.normal = glm::normalize(glm::cross(b-a, c-a));


	return report;

}

inline IntersectionReport optIntersectBVH(Ray r, Mesh mesh, const std::vector<BVHNode>& BVHNodes, const std::vector<Indices>& BVHIndices, const std::vector<Vertex>& BVHNormals, const std::vector<Vertex>& vertexData, float tmin, float tmax, float intersectionTestEpsilon)
{
    float tMin = FLT_MAX;
    IntersectionReport report;
    report.dbg = 0;
    report.intersection = glm::vec3(FLT_MAX);
    report.d = tMin;
    report.materialId = mesh.materialId;

    int nodeIndexNext = mesh.rootOffset;
    int size = mesh.rootOffset + mesh.nodeSize;

    for(int nodeIndex = mesh.rootOffset; nodeIndex < size; nodeIndex++)
    {
        if(nodeIndex != nodeIndexNext)
            continue;

        bool nodeIntersection = intersectBox(r, BVHNodes[nodeIndex].aabb, tmin, tmax);
        bool isLeaf = BVHNodes[nodeIndex].childSize == 1;

        if(nodeIntersection == true)
        {
            nodeIndexNext += 1;
        }
        else
        {
            nodeIndexNext += BVHNodes[nodeIndex].childSize;
        }

        if(nodeIntersection == true && isLeaf)
        {
            int offset = BVHNodes[nodeIndex].indicesOffset;
            int offsetSize = offset + BVHNodes[nodeIndex].indicesSize;

            for(int j = offset; j < offsetSize; j++)
            {
                IntersectionReport rprt = intersectFace(r, BVHIndices[j], tmin, tmax, vertexData, intersectionTestEpsilon);
                if(rprt.d < tMin)
                {
                    report.intersection = rprt.intersection;
                    report.normal = BVHNormals[j].pos;
                    report.d = rprt.d;
                    tMin = report.d;
                }
            }
        }
    }

    return report;
}

inline IntersectionReport testWorldIntersection(Ray primaryRay, 
                                         const std::vector<BVHNode>& BVHNodes,
                                         const std::vector<Indices>& BVHIndices,
                                         const std::vector<Vertex>& BVHNormals,
                                         const std::vector<Vertex>& vertexData,
                                         const std::vector<Mesh>& meshes,
                                         const std::vector<Triangle>& triangles,
                                         const std::vector<Sphere>& spheres,
                                         float tmin,
                                         float tmax,
                                         int meshCount,
                                         int triangleCount,
                                         int sphereCount,
                                         float intersectionTestEpsilon)
{
    float tMin = FLT_MAX;
    IntersectionReport report;
    report.intersection = glm::vec3(FLT_MAX);
    report.d = FLT_MAX;
    report.dbg = 1;

    for(int i=0; i < meshCount; i++)
    {
        IntersectionReport r = optIntersectBVH(primaryRay, meshes[i], BVHNodes, BVHIndices, BVHNormals, vertexData, tmin, tmax, intersectionTestEpsilon);
        if(r.d < tMin)
        {
            report = r;
            tMin = r.d;
        }

    }

    for(int i=0; i < triangleCount; i++)
    {
        IntersectionReport r = intersectTriangle(primaryRay, triangles[i], vertexData, tmin, tmax, intersectionTestEpsilon);
        if(r.d < tMin)
        {
            report = r;
            tMin   = r.d;
        }
    }

    for(int i=0; i < sphereCount; i++)
    {
        IntersectionReport r = intersectSphere(primaryRay, spheres[i], tmin, tmax, vertexData);
        if(r.d < tMin)
        {
            report = r;
            tMin = r.d;
        }
    }

    return report;
}

inline bool shadowRayIntersection(Ray ray, float dist,
                                  const std::vector<BVHNode>& BVHNodes,
                                  const std::vector<Indices>& BVHIndices,
                                  const std::vector<Vertex>& BVHNormals,
                                  const std::vector<Vertex>& vertexData,
                                  const std::vector<Mesh>& meshes,
                                  const std::vector<Triangle>& triangles,
                                  const std::vector<Sphere>& spheres,
                                  float tmin,
                                  float tmax,
                                  int meshCount,
                                  int triangleCount,
                                  int sphereCount,
                                  float intersectionTestEpsilon)
{

    float tMin = FLT_MAX;
    IntersectionReport report;
    report.intersection = glm::vec3(FLT_MAX);
    report.d = FLT_MAX;

    for(int i=0; i < meshCount; i++)
    {
        IntersectionReport r = optIntersectBVH(ray, meshes[i], BVHNodes, BVHIndices, BVHNormals, vertexData, tmin, tmax, intersectionTestEpsilon);
        if(r.d < dist)
        {
            return true;
        }

    }

    for(int i=0; i < triangleCount; i++)
    {
        IntersectionReport r = intersectTriangle(ray, triangles[i], vertexData, tmin, tmax, intersectionTestEpsilon);
        if(r.d < dist)
        {
            return true;
        }
    }

    for(int i=0; i < sphereCount; i++)
    {
        IntersectionReport r = intersectSphere(ray, spheres[i], tmin, tmax, vertexData);
        if(r.d < tMin)
        {
            return true;
        }
    }

    return false;
}


#endif