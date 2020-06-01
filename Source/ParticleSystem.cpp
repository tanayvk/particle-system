#include "ParticleSystem.h"
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include "Util.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ctime>
#include <cstdlib>

ParticleSystem::ParticleSystem()
{

}

bool ParticleSystem::Initialize(int poolSize)
{
    glEnable(GL_BLEND);
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

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);
    glLinkProgram(ShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Particles.resize(poolSize);

    return 1;
}


void ParticleSystem::Update(float deltaMs)
{
    int count = 0;
    for (Particle& particle: Particles)
    {
        if ( !particle.Active )
            continue;

        particle.RemainingLife -= deltaMs;
        if ( particle.RemainingLife <= 1.0f )
        {
            particle.Active = false;
            continue;
        }

        particle.Rotation += particle.RotationSpeed * deltaMs;
        particle.Position += particle.Velocity * deltaMs;
    }
}

void ParticleSystem::Render()
{
    glUseProgram(ShaderProgram);
    ShaderColorLocation = glGetUniformLocation(ShaderProgram, "Color");
    ShaderTransformLocation = glGetUniformLocation(ShaderProgram, "Transform");

    for (Particle& particle: Particles)
    {	
        if ( !particle.Active )
            continue;
        float life = particle.RemainingLife / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.EndColor, particle.StartColor, life);

		float size = glm::lerp(particle.EndSize, particle.StartSize, life);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
			                  * glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			                  * glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
        glUniform4fv(ShaderColorLocation, 1, glm::value_ptr(color));
        glUniformMatrix4fv(ShaderTransformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void ParticleSystem::Emit(ParticleProps props)
{
    for (Particle& particle: Particles)
    {
        if ( particle.Active )
            continue;

        particle.RemainingLife = particle.LifeTime = props.LifeTime;
        particle.StartColor = props.StartColor;
        particle.EndColor = props.EndColor;
        particle.StartSize = props.StartSize;
        particle.EndSize = props.EndSize;
        particle.Position = props.Position;

        particle.RotationSpeed = 1.0f;
        particle.Velocity = props.Velocity;
        particle.VelocityVariation = props.VelocityVariation;
        particle.Velocity += glm::vec2(((float)(std::rand() % 5000)/2500 - 1.0f) * particle.VelocityVariation.x,
                                       ((float)(std::rand() % 5000)/2500 - 1.0f) * particle.VelocityVariation.y);
        particle.Active = true;

        break;
    }
}

ParticleSystem::~ParticleSystem()
{
    
}
