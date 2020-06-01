#include "Sandbox.h"
#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include <algorithm>
#include <stdio.h>
#include "Util.h"
#include "ParticleSystem.h"

#include <glm/glm.hpp>

const int windowHeight = 1000;
const int windowWidth = 1000;

Sandbox::Sandbox()
{
    SDLIsInitialized = false;
    Window = nullptr;
    TicksCount = 0;
    Particles = nullptr;
    MousePressed = false;
    lastMouseX = lastMouseY = 0;
}

bool Sandbox::Initialize()
{
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        return 0;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    Window = SDL_CreateWindow("Particles!",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              windowWidth,
                              windowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    if ( Window == nullptr )
    {
        return 0;
    }

    AmIRunning = true;
    
    Context = SDL_GL_CreateContext( Window );
    unsigned int glewError = glewInit();

    if ( glewError != GLEW_OK )
    {
        SDL_Log("%s", glewGetErrorString(glewError));
        return 0;
    }
    SDL_Log("Initialized everything fine!\nNow doing graphics!");

    Particles = new ParticleSystem();
    Particles->Initialize(1000);

    SDL_Log("Initialization successful!");
    return 1;
}

void Sandbox::CleanUp()
{
    if (Particles != nullptr )
        delete Particles;
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
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                MousePressed = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                MousePressed = false;
            break;
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

    if ( MousePressed && TicksCount % 3 > 0)
    {
        int x, y, mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_GetMouseState(&x, &y);
        x -= windowWidth/2;
        y = -y + windowHeight/2;

        ParticleProps props;
        props.LifeTime = 2.0f;
        props.StartColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
        props.EndColor = glm::vec4(0.0f, 1.0f, 1.0f, 0.0f);
        props.StartSize = 0.1f;
        props.EndSize = 0.01f;
        props.Position = glm::vec2(2.0f*(float)x/windowWidth, 2.0f*(float)y/windowHeight);
        props.Velocity = 0.5f * glm::normalize(glm::vec2((float) lastMouseX - (float) mouseX, (float) mouseY - (float) lastMouseY));
        
        props.VelocityVariation = glm::vec2(0.3f, 0.3f);
        SDL_Log("particle velocity %f, %f", props.Velocity.x, props.Velocity.y);
        // velocity, velocity variation & rotation speed
        Particles->Emit(props);
    }

    Particles->Update(deltaTime);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        SDL_Log("%s", glewGetErrorString(err));
    }

    SDL_GetMouseState(&lastMouseX, &lastMouseY);
}

void Sandbox::Render()
{
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    Particles->Render();

    SDL_GL_SwapWindow(Window);
}
