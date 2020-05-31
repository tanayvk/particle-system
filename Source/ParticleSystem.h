#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct ParticleProps {
    int LifeTime;
    glm::vec4 StartColor, EndColor;
    float StartSize, EndSize;
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    float RotationSpeed;
};

class ParticleSystem {
public:
    ParticleSystem();
    bool Initialize(int poolSize);

    void Update(float deltaMs);
    void Render();

    void Emit(ParticleProps props);

    ~ParticleSystem();
private:
    struct Particle {
        bool Active = false;

        float LifeTime, RemainingLife;

        glm::vec4 StartColor, EndColor;
        float StartSize, EndSize;

        glm::vec2 Position;
        glm::vec2 Velocity, VelocityVariation;
        float Rotation = 0.0f;
        float RotationSpeed;
    };

    int PoolSize;
    std::vector<Particle> Particles;

    unsigned int VAO;
    unsigned int ShaderProgram;

    unsigned int ShaderColorLocation;
    unsigned int ShaderTransformLocation;
};
