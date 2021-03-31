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

Scene& SceneManager::GetScene(int index)
{
    return _scenes[index];
}