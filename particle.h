#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <shader.h>
#include <texture.h>
#include <vector>
#include <gameobject.h>
#include <random>
#include <ball.h>
#include <block.h>

class Particle
{

public:

    Particle():
        color(0), size(0), position(0), velocity(0), life(0)
    {}

    glm::vec4 color;
    glm::vec2 size, position, velocity;
    float life;
};

// #####################

class Particle_generator
{

public:

    Particle_generator(Shader* shader, Texture* texture, int max_particles, const glm::vec4& color, const glm::vec2& size,
                       const glm::vec2& spawn_range, float full_life, float max_new_perFrame);

    ~Particle_generator();

    std::vector<Particle> particles;

    void draw();

    void update(float dT, Ball& ball);

    // some particle data
    glm::vec4 color;
    glm::vec2 size, spawn_range;
    float full_life, max_new_perFrame;
    float life;
    bool isActive;
    bool fire = true;

    void update(float dT);
    void respawn_particle(Particle& particle, Block& object);

    Block *obj;

    Particle_generator& operator=(const Particle_generator&) = delete;
    Particle_generator(const Particle_generator& temp):
        shader(temp.shader),
        texture(temp.texture)
    {
        this->max_particles = temp.max_particles;



        particles.reserve(this->max_particles);
        for(unsigned int i = 0; i < this->max_particles; ++i)
        {
            particles.push_back(Particle());
        }

        init_draw_fun();
        last_used_particle = 0;

        this->color = temp.color;
        this->size = temp.size;
        this->spawn_range = temp.spawn_range;
        this->full_life = temp.full_life;
        this->max_new_perFrame = temp.max_new_perFrame;
        this->fire = true;

        this->life = 4;
        isActive = false;

    }



private:

    Shader* shader;

    Texture* texture;

    unsigned int max_particles, last_used_particle;



    // dangerous resources
    GLuint VAO, VBO;

    // functions

    int find_free_place();

    void respawn_particle(Particle& particle, Ball& ball);

    void init_draw_fun();
};

#endif // PARTICLE_H
