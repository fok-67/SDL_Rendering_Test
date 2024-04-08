#pragma once

#include <thread>
#include <memory>
#include <atomic>

#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer(SDL_Window* window);

    ~Renderer() = default;

    void stop();

private:
    void work();
    void render(int w, int h);

    SDL_GLContext m_Context;
    SDL_Renderer* m_Renderer;
    SDL_Window* m_Window;
    std::unique_ptr<std::thread> m_Thread;
    std::atomic_flag m_RenderInit;
    std::atomic_flag m_Stop;
};
