#include <vector>

class ParticleSystem {
public:
    ParticleSystem();
private:
    struct Particle {
        bool Active;
    };
    int PoolSize;
    std::vector<Particle> Particles;
};
