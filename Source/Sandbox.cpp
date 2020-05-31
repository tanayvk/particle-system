#include "Sandbox.h"
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <algorithm>

const int windowHeight = 800;
const int windowWidth = 800;

Sandbox::Sandbox()
{
    SDLIsInitialized = false;
    Window = nullptr;
    TicksCount = 0;
}

bool Sandbox::Initialize()
{
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        return 0;
    }

    Window = SDL_CreateWindow("Top Down Sandbox",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            windowWidth,
                            windowHeight,
                            SDL_WINDOW_OPENGL);

    if ( Window == nullptr )
    {
        return 0;
    }

    Context = SDL_GL_CreateContext(Window);

    AmIRunning = true;
    SDL_Log("Initialized everything fine!");
    
    return 1;
}

void Sandbox::CleanUp()
{
    if ( Window != nullptr )
        SDL_DestroyWindow(Window);
    if (SDLIsInitialized)
        SDL_Quit();
}

void Sandbox::Loop()
{
    ProcessInput();
    Update();
    Render();
}

void Sandbox::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT: 
            AmIRunning = false;
        }
    }
}

void Sandbox::Update()
{
    // fps cap
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16))
        ;

    // clamp delta time to avoid exceptionally big values
    float deltaTime = (SDL_GetTicks() - TicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    TicksCount = SDL_GetTicks();
}

void Sandbox::Render()
{
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(1.f, 0.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(Window);
}
