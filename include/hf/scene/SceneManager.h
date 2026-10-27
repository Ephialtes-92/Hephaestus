#pragma once

#include <memory>

#include <hf/scene/IScene.h>

namespace hf::scene
{
class SceneManager
{
public:
    void ChangeScene(std::unique_ptr<IScene> nextScene);
    void Update(float deltaTime);
    void Render();
    void UnloadCurrent();
    IScene* GetCurrentScene() const;

private:
    std::unique_ptr<IScene> m_CurrentScene;
};
}
