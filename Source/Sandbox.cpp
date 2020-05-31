#include "Sandbox.h"
#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include <algorithm>
#include <stdio.h>

const int windowHeight = 1000;
const int windowWidth = 1000;

Sandbox::Sandbox()
{
    SDLIsInitialized = false;
    Window = nullptr;
    TicksCount = 0;
}

char* readFileToString(char* filename)
{
    FILE* f = fopen(filename, "rb");
    fseek (f, 0, SEEK_END);
    long length = ftell (f);
    fseek (f, 0, SEEK_SET);
    char* string = (char*)malloc (length);
    fread (string, 1, length, f);
    fclose (f);
    return string;
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

    Window = SDL_CreateWindow("Top Down Sandbox",
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

    // graphics init
	float vertices[] = {
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 3, 2,
	};
    
	unsigned int VBO;
    glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// compile and link shaders

    char* vertexShaderSource = readFileToString("Particles.vert");
    char* fragmentShaderSource = readFileToString("Particles.frag");
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
	
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		SDL_Log("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		SDL_Log("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        SDL_Log("%s", glewGetErrorString(err));
    }
}

void Sandbox::Render()
{
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    int colorLocation = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLocation, 1.0f, 0.5f, 1.0f, 1.0f);

    glBindVertexArray(VAO);
    
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    SDL_GL_SwapWindow(Window);
}
