#pragma once

struct GLFWwindow;

namespace hf::scene
{
class IScene
{
public:
    virtual ~IScene() = default;

    virtual void Load() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Unload() = 0;

    virtual void HandleKey(GLFWwindow* window, int key, int action) {}
    virtual void HandleMouseMove(double xpos, double ypos) {}
    virtual void HandleScroll(double xoffset, double yoffset) {}
    virtual void HandleResize(int width, int height) {}
};
}
