#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <string>
#include <Scene.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <Program.h>

class SceneManager
{
private:
    std::vector<Scene> _scenes;

public:

    SceneManager();
    ~SceneManager();

    void AddScene(const std::string& path);
    void LoadScene(int index, ComputeProgram* program);
    void ExitScene(int index);

    Scene& GetScene(int index);
};


#endif /* __SCENE_MANAGER_H__ */