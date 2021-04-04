#include <Scene.h>


Scene::Scene(const std::string& filepath)
{
    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if(res)
    {
        throw std::runtime_error("Error: The xml file cannot be loaded");
    }

    auto root = file.FirstChild();
    if(!root)
    {
        throw std::runtime_error("Error: Root is not found.");
    }

    auto element = root->FirstChildElement("BackgroundColor");
    if(element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0 0 0" << std::endl;
    }

    stream >> _backgroundColor.r >> _backgroundColor.g >> _backgroundColor.b;

    element = root->FirstChildElement("ShadowRayEpsilon");
    if(element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0.001" << std::endl;
    }
    stream >> _shadowRayEpsilon;

    element = root->FirstChildElement("IntersectionTestEpsilon");
    if(element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0.000001" << std::endl;
    }
    stream >> _intersectionTestEpsilon;

    element = root->FirstChildElement("MaxRecursionDepth");
    if(element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0" << std::endl;
    }
    stream >> _maxRecursionDepth;

    //Get Cameras
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera;
    while(element)
    {
        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Gaze");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearPlane");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;

        stream >> camera.position.x >> camera.position.y >> camera.position.z;
        stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        stream >> camera.nearPlane.x >> camera.nearPlane.y >> camera.nearPlane.z >> camera.nearPlane.w;
        stream >> camera.nearDistance;
        stream >> camera.imageResolution.x >> camera.imageResolution.y;

        // normalize gaze and up and compute v
        camera.gaze = glm::normalize(camera.gaze);
        camera.up   = glm::normalize(camera.up);
        camera.v    = glm::normalize(glm::cross(camera.gaze, camera.up));

        std::string imageName;
        stream >> imageName;
        imageNames.push_back(_imageName);


        _cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    // Get Lights
    element = root->FirstChildElement("Lights");
    auto child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> _ambientLight.x >> _ambientLight.y >> _ambientLight.z;
    _ambientLight.x /= 255.99;
    _ambientLight.y /= 255.99;
    _ambientLight.z /= 255.99;
    element = element->FirstChildElement("PointLight");
    PointLight pointLight;
    while(element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> pointLight.position.x >> pointLight.position.y >> pointLight.position.z;
        stream >> pointLight.intensity.r >> pointLight.intensity.g >> pointLight.intensity.b;

        _pointLights.push_back(pointLight);
        element = element->NextSiblingElement("PointLight");
    }

    // Get Materials
    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    Material material;
    while(element)
    {
        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        //child = element->FirstChildElement("MirrorReflectance");
        //stream << child->GetText() << std::endl;
        child = element->FirstChildElement("PhongExponent");
        if(child)
        {
            stream << child->GetText() << std::endl;
        }
        else
        {
            stream << "1" << std::endl;
        }


        stream >> material.ambientReflectance.x >> material.ambientReflectance.y >> material.ambientReflectance.z;
        stream >> material.diffuseReflectance.x >> material.diffuseReflectance.y >> material.diffuseReflectance.z;
        stream >> material.specularReflectance.x >> material.specularReflectance.y >> material.specularReflectance.z;
        //stream >> material.mirrorReflectance.x >> material.mirrorReflectance.y >> material.mirrorReflectance.z;
        stream >> material.phongExponent;

        _materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }
    stream.clear();
    // Get VertexData
    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    Vertex vertex;
    while(!(stream >> vertex.pos.x).eof())
    {
        stream >> vertex.pos.y >> vertex.pos.z;
        _vertexData.push_back(vertex);
    }
    stream.clear();

    // Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    Mesh mesh;
    int offset = 0;
    while(element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> mesh.materialId;

        child = element->FirstChildElement("Faces");
        stream << child->GetText() << std::endl;
        Indices indices;
        mesh.indicesOffset = offset;
        while(!(stream >> indices.a).eof())
        {
            stream >> indices.b >> indices.c;
            _meshIndexBuffer.push_back(indices);
            offset++;
        }
        mesh.indicesSize = offset;
        stream.clear();

        _meshes.push_back(mesh);
        element = element->NextSiblingElement("Mesh");
    }
    stream.clear();

    // Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");
    Triangle triangle;
    while(element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> triangle.materialId;

        child = element->FirstChildElement("Indices");
        stream << child->GetText() << std::endl;
        stream >> triangle.indices.a >> triangle.indices.b >> triangle.indices.c;

        _triangles.push_back(triangle);
        element = element->NextSiblingElement("Triangle");
    }

    // Get Spheres
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    Sphere sphere;
    while(element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> sphere.materialId;

        child = element->FirstChildElement("Center");
        stream << child->GetText() << std::endl;
        stream >> sphere.centerVertexId;

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << std::endl;
        stream >> sphere.radius;

        _spheres.push_back(sphere);
        element = element->NextSiblingElement("Sphere");
    }

    _activeCamera = _cameras[0];

}

Scene::~Scene()
{
    
}

void Scene::ComputeFaceNormals()
{
    for(size_t i=0; i<_meshIndexBuffer.size(); i++)
    {
        Indices faceIndices = _meshIndexBuffer[i];
        Vertex normalElement;
        glm::vec3 a = _vertexData[faceIndices.a-1].pos;
        glm::vec3 b = _vertexData[faceIndices.b-1].pos;
        glm::vec3 c = _vertexData[faceIndices.c-1].pos;

        glm::vec3 normal = glm::cross((b-a),(c-a));
        normal = glm::normalize(normal);
        normalElement.pos = normal;

        _meshNormals.push_back(normalElement);
    }
}


void Scene::BindObjectsToGPU()
{

    // Bind point light array
    glGenBuffers(1, &ssbo_pointLights);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pointLights);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _pointLights.size() * sizeof(PointLight), _pointLights.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_pointLights);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    
    // Bind materials array
    glGenBuffers(1, &ssbo_materials);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_materials);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _materials.size() * sizeof(Material), _materials.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_materials);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Bind vertexData array
    glGenBuffers(1, &ssbo_vertexData);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vertexData);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _vertexData.size() * sizeof(Vertex), _vertexData.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo_vertexData);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); 

    // Bind meshes array
    glGenBuffers(1, &ssbo_meshes);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_meshes);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _meshes.size() * sizeof(Mesh), _meshes.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo_meshes);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Bind triangles array
    glGenBuffers(1, &ssbo_triangles);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_triangles);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _triangles.size() * sizeof(Triangle), _triangles.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ssbo_triangles);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    // Bind spheres array
    glGenBuffers(1, &ssbo_spheres);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_spheres);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _spheres.size() * sizeof(Sphere), _spheres.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, ssbo_spheres);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Bind cameras array   
    glGenBuffers(1, &ssbo_cameras);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_cameras);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _cameras.size() * sizeof(Camera), _cameras.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, ssbo_cameras);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    // Bind indices array   
    glGenBuffers(1, &ssbo_meshIndices);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_meshIndices);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _meshIndexBuffer.size() * sizeof(Indices), _meshIndexBuffer.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, ssbo_meshIndices);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Bind mesh normals array   
    glGenBuffers(1, &ssbo_meshNormals);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_meshNormals);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _meshNormals.size() * sizeof(Vertex), _meshNormals.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, ssbo_meshNormals);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);    

}

void Scene::SetUniforms(ComputeProgram* program)
{
    glUseProgram(program->id);

    // Set background color and ambient lights
    glUniform3f(glGetUniformLocation(program->id, "backgroundColor"), _backgroundColor.x, _backgroundColor.y, _backgroundColor.z);
    glUniform3f(glGetUniformLocation(program->id, "ambientLight"), _ambientLight.x, _ambientLight.y, _ambientLight.z);

    // Set camera properties
    glUniform3f(glGetUniformLocation(program->id, "camera.position"), _activeCamera.position.x, _activeCamera.position.y, _activeCamera.position.z);
    glUniform3f(glGetUniformLocation(program->id, "camera.gaze"), _activeCamera.gaze.x, _activeCamera.gaze.y, _activeCamera.gaze.z);
    glUniform3f(glGetUniformLocation(program->id, "camera.up"), _activeCamera.up.x, _activeCamera.up.y, _activeCamera.up.z);
    glUniform3f(glGetUniformLocation(program->id, "camera.v"), _activeCamera.v.x, _activeCamera.v.y, _activeCamera.v.z);
    glUniform4f(glGetUniformLocation(program->id, "camera.nearPlane"), _activeCamera.nearPlane.x, _activeCamera.nearPlane.y, _activeCamera.nearPlane.z, _activeCamera.nearPlane.w);
    glUniform2f(glGetUniformLocation(program->id, "camera.imageResolution"), _activeCamera.imageResolution.x, _activeCamera.imageResolution.y);
    glUniform1f(glGetUniformLocation(program->id, "camera.nearDistance"), _activeCamera.nearDistance);

    // Set max recursion depth
    glUniform1i(glGetUniformLocation(program->id, "maxRecursionDepth"), _maxRecursionDepth);

    // set shadow ray epsilon
    glUniform1f(glGetUniformLocation(program->id, "shadowRayEpsilon"), _shadowRayEpsilon);

    // set intersection test epsilon
    glUniform1f(glGetUniformLocation(program->id, "intersectionTestEpsilon"), _intersectionTestEpsilon);

    // set sizes of ssbo objects
    glUniform1i(glGetUniformLocation(program->id, "lightCount"), _pointLights.size());
    glUniform1i(glGetUniformLocation(program->id, "materialCount"), _materials.size());
    glUniform1i(glGetUniformLocation(program->id, "vertexCount"), _vertexData.size());
    glUniform1i(glGetUniformLocation(program->id, "meshCount"), _meshes.size());
    glUniform1i(glGetUniformLocation(program->id, "meshIndexCount"), _meshIndexBuffer.size());
    glUniform1i(glGetUniformLocation(program->id, "triangleCount"), _triangles.size());
    glUniform1i(glGetUniformLocation(program->id, "sphereCount"), _spheres.size());                    




}