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
    _scenes[index].BindObjectsToGPU();
    _scenes[index].SetUniforms(program);
}

void SceneManager::ExitScene(int index)
{

}


Scene& SceneManager::GetScene(int index)
{
    return _scenes[index];
}
