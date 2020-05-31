#include <SDL.h>
#include <vector>

class Sandbox {
public:
    Sandbox();
    bool Initialize();
    void Loop();
    void CleanUp();

    bool IsRunning() { return AmIRunning; }
private:
    void ProcessInput();
    void Update();
    void Render();

    int TicksCount;
    bool AmIRunning;

    // SDL
    bool SDLIsInitialized;
    SDL_Window* Window;
    SDL_GLContext Context;

    unsigned int VAO;
    unsigned int shaderProgram;
};
