#include "Renderer.h"

#include <iostream>
#include <format>

Renderer::Renderer(SDL_Window* window)
    : m_Renderer{ nullptr }
    , m_Window{ window }
{
    m_Context = SDL_GL_GetCurrentContext();

    // Start the rendering thread.
    m_Thread = std::make_unique<std::thread>(&Renderer::work, this);

    // Wait for the renderer to be initialized.
    m_RenderInit.wait(false);
}

void Renderer::stop()
{
    m_Stop.test_and_set();
    m_Thread->join();
}

void Renderer::render(int width, int height)
{
    // Clear
    //
    if ((SDL_SetRenderDrawColor(m_Renderer, 0, 0x38, 0x7b, 0xff) < 0) ||
        (SDL_RenderClear(m_Renderer) < 0))
    {
        std::cerr << std::format("Error clearing: {}\n", SDL_GetError());
    }

    // Draw
    //
    SDL_Rect rect{ (width/2) - 20, (height/2) - 20, 40, 40 };

    if ((SDL_SetRenderDrawColor(m_Renderer, 0xff, 0xff, 0, 0xff) < 0) ||
        (SDL_RenderFillRect(m_Renderer, &rect) < 0))
    {
        std::cerr << std::format("Error drawing: {}\n", SDL_GetError());
    }

    // Display
    //
    SDL_RenderPresent(m_Renderer);

    // Cap framerate
    SDL_Delay(16);
}

void Renderer::work()
{
    while (! m_Stop.test())
    {
        if (! m_RenderInit.test())
        {
            SDL_GL_MakeCurrent(m_Window, m_Context);

            m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
            if (! m_Renderer)
            {
                std::cerr << std::format("Cannot create the renderer: {}\n", SDL_GetError());
                break;
            }

            // Signal that the renderer has been initialized.
            m_RenderInit.test_and_set();
            m_RenderInit.notify_one();
        }
        else
        {
            int w, h;
            SDL_GetWindowSize(m_Window, &w, &h);

            render(w, h);
        }
    }

    // Clean up
    SDL_DestroyRenderer(m_Renderer);
}


