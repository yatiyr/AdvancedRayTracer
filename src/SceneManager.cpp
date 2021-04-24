#include <SceneManager.h>


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
    
}

void SceneManager::AddScene(const std::string& path)
{
    Scene x(path);
    _scenes.push_back(x);
}

void SceneManager::LoadScene(int index, ComputeProgram* program)
{
    _scenes[index].ComputeFaceNormals();
    _scenes[index].BindObjectsToGPU();
    _scenes[index].SetUniforms(program);
    _activeSceneID = index;
    
}

void SceneManager::LoadSceneForCPU(int index)
{
    _scenes[index].ComputeFaceNormals();
}

void SceneManager::ExitScene(int index)
{

}


Scene& SceneManager::GetScene(int index)
{
    return _scenes[index];
}

Scene* SceneManager::GetScenePointer(int index)
{
    return &_scenes[index];
}


Scene& SceneManager::GetActiveScene()
{
    return _scenes[_activeSceneID];
}
