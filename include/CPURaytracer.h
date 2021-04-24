#ifndef __CPU_RAY_TRACER__
#define __CPU_RAY_TRACER__


#include <Structures.h>
#include <CPUIntersections.h>

inline glm::vec3 computeAmbientComponent(IntersectionReport report, glm::vec3 ambientLight, const std::vector<Material>& materials)
{
    return ambientLight * materials[report.materialId - 1].ambientReflectance;
}

inline glm::vec3 computeDiffuseComponent(IntersectionReport report, PointLight light, const std::vector<Material>& materials)
{
    glm::vec3 result = glm::vec3(0.0);

    float lightDistance = glm::length(light.position - report.intersection);
    glm::vec3 wi = glm::normalize(light.position - report.intersection);
    result += materials[report.materialId - 1].diffuseReflectance * 
              std::max(0.0f, glm::dot(wi, report.normal)) *
              (light.intensity/(lightDistance*lightDistance));

    return result;

}

inline glm::vec3 computeSpecularComponent(IntersectionReport report, Ray primaryRay, PointLight light, const std::vector<Material>& materials)
{
    glm::vec3 result = glm::vec3(0.0);

    float lightDistance = glm::length(light.position - report.intersection);
    glm::vec3 wi = glm::normalize(light.position - report.intersection);
    glm::vec3 h  = glm::normalize(wi - primaryRay.direction);

    result += materials[report.materialId - 1].specularReflectance *
              std::pow(std::max(0.0f, glm::dot(report.normal, h)), materials[report.materialId - 1].phongExponent) *
              (light.intensity/(lightDistance*lightDistance));

    return result;
}

inline bool shadowTest(IntersectionReport report, PointLight light, float shadowRayEpsilon,
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
    Ray lightRay;

    lightRay.direction = glm::normalize(light.position - report.intersection);
    lightRay.origin = report.intersection + shadowRayEpsilon*report.normal;

    float dist = glm::length(light.position - report.intersection);

    return shadowRayIntersection(lightRay, dist,
                                 BVHNodes,
                                 BVHIndices,
                                 BVHNormals,
                                 vertexData,
                                 meshes,
                                 triangles,
                                 spheres,
                                 tmin,
                                 tmax,
                                 meshCount,
                                 triangleCount,
                                 sphereCount,
                                 intersectionTestEpsilon);
}


inline glm::vec3 computeAmbientDiffuseSpecular(IntersectionReport report, Ray primaryRay,
                                                float shadowRayEpsilon,
                                                glm::vec3 ambientLight,
                                                const std::vector<BVHNode>& BVHNodes,
                                                const std::vector<Indices>& BVHIndices,
                                                const std::vector<Vertex>& BVHNormals,
                                                const std::vector<Vertex>& vertexData,
                                                const std::vector<Mesh>& meshes,
                                                const std::vector<Triangle>& triangles,
                                                const std::vector<Sphere>& spheres,
                                                const std::vector<PointLight>& pointLights,
                                                const std::vector<Material>& materials,
                                                float tmin,
                                                float tmax,
                                                int meshCount,
                                                int triangleCount,
                                                int sphereCount,
                                                int pointLightCount,
                                                float intersectionTestEpsilon)

{
    glm::vec3 result = computeAmbientComponent(report, ambientLight, materials);

    for(int i=0; i<pointLightCount; i++)
    {
        //if(shadowTest(report, pointLights[i], shadowRayEpsilon, BVHNodes,
          //            BVHIndices, BVHNormals, vertexData, meshes,
            //          triangles, spheres, tmin, tmax, meshCount, triangleCount,
            //          sphereCount, intersectionTestEpsilon))
        //{
          //  continue;
        //}
        //else
        //{
            result += computeDiffuseComponent(report, pointLights[i], materials) +
                      computeSpecularComponent(report, primaryRay, pointLights[i], materials);
        //}
    }


    return glm::clamp(result, glm::vec3(0.0f), glm::vec3(1.0f));
}



inline Ray ComputePrimaryRay(int i, int j, const Camera& cam)
{
    Ray r;
    r.origin = cam.position;
    glm::vec3 m = r.origin + cam.gaze * cam.nearDistance;
    glm::vec3 q = m + cam.nearPlane.x * cam.v + cam.nearPlane.w * cam.up;

    float su = (j + 0.5) * (cam.nearPlane.y - cam.nearPlane.x) / cam.imageResolution.x;
    float sv = (i + 0.5) * (cam.nearPlane.w - cam.nearPlane.z) / cam.imageResolution.y;

    r.direction = glm::normalize((q + su*cam.v - sv*cam.up) - r.origin);

    return r;
}

inline glm::vec3 RayTrace(Ray r, Scene *scene)
{
    IntersectionReport report = testWorldIntersection(r, scene->_BVHNodes, scene->_BVHIndices, scene->_BVHNormals,
                                                      scene->_vertexData, scene->_meshes, scene->_triangles,
                                                      scene->_spheres, 0.001, 2000, scene->_meshes.size(), scene->_triangles.size(), 
                                                      scene->_spheres.size(), scene->_intersectionTestEpsilon);

    glm::vec3 pixel(0.0f);

    if(report.d != FLT_MAX)
    {
        pixel = computeAmbientDiffuseSpecular(report, r, scene->_shadowRayEpsilon,
                                              scene->_ambientLight, scene->_BVHNodes,
                                              scene->_BVHIndices, scene->_BVHNormals,
                                              scene->_vertexData, scene->_meshes, scene->_triangles,
                                              scene->_spheres, scene->_pointLights, scene->_materials,
                                              0.001, 2000, scene->_meshes.size(), scene->_triangles.size(),
                                              scene->_spheres.size(), scene->_pointLights.size(),
                                              scene->_intersectionTestEpsilon);
    }
    else
    {
        pixel = scene->_backgroundColor;
    }


    return pixel;


}


#endif