#include <hf/scene/SceneManager.h>

namespace hf::scene
{

void SceneManager::ChangeScene(std::unique_ptr<IScene> nextScene)
{
    if (m_CurrentScene)
        m_CurrentScene->Unload();

    m_CurrentScene = std::move(nextScene);
    if (m_CurrentScene)
        m_CurrentScene->Load();
}

void SceneManager::Update(float deltaTime)
{
    if (m_CurrentScene)
        m_CurrentScene->Update(deltaTime);
}

void SceneManager::Render()
{
    if (m_CurrentScene)
        m_CurrentScene->Render();
}

void SceneManager::UnloadCurrent()
{
    if (m_CurrentScene)
    {
        m_CurrentScene->Unload();
        m_CurrentScene.reset();
    }
}

IScene* SceneManager::GetCurrentScene() const
{
    return m_CurrentScene.get();
}

}