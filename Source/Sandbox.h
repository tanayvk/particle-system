#pragma once

#include <SDL.h>

class Sandbox {
public:
    Sandbox();
    bool Initialize();
    void Loop();
    void CleanUp();

    bool IsRunning() { return AmIRunning; }
private:
    void ProcessInput();
    bool MousePressed;

    void Update();
    void Render();

    int TicksCount;
    bool AmIRunning;

    // SDL
    bool SDLIsInitialized;
    SDL_Window* Window;
    SDL_GLContext Context;

    class ParticleSystem* Particles;
};
