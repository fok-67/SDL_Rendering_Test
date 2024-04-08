#include <iostream>
#include <format>
#include <SDL2/SDL.h>

#include "Renderer.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* window = SDL_CreateWindow("Render test",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          500, 500,
                                          SDL_WINDOW_RESIZABLE);
    if (! window)
    {
        std::cerr << std::format("Cannot create the window: {}\n", SDL_GetError());
        return 1;
    }

    Renderer renderer(window);

    while (true)
    {
        SDL_Event event;

        if (SDL_WaitEvent(&event))
        {
            if ((event.type == SDL_QUIT) || 
                ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)))
            {
                renderer.stop();
                break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
