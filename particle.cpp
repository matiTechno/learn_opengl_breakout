#include "particle.h"
#include <glm/gtx/transform.hpp>
#include <gameobject.h>

Particle_generator::Particle_generator(Shader* shader, Texture* texture, int max_particles, const glm::vec4& color, const glm::vec2& size,
                                       const glm::vec2& spawn_range, float full_life, float max_new_perFrame):
    shader(shader),
    texture(texture)
{
    this->max_particles = max_particles;

    particles.reserve(this->max_particles);
    for(unsigned int i = 0; i < this->max_particles; ++i)
    {
        particles.push_back(Particle());
    }

    init_draw_fun();
    last_used_particle = 0;

    this->color = color;
    this->size = size;
    this->spawn_range = spawn_range;
    this->full_life = full_life;
    this->max_new_perFrame = max_new_perFrame;

    this->life = 0;
    isActive = false;
}

Particle_generator::~Particle_generator()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Particle_generator::draw()
{
    for(std::vector<Particle>::iterator i = particles.begin(); i != particles.end(); ++i)
    {
        if(i->life > 0)
        {
            texture->bind();

            glm::mat4 model(1.f);
            // translation
            model = glm::translate(model, glm::vec3(i->position, 0));
            //scaling
            model = glm::scale(model, glm::vec3(i->size, 1));

            if(color == glm::vec4(0, 0, 0, 1))
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            else
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            shader->bind();
            glUniformMatrix4fv(glGetUniformLocation(shader->program, "model"), 1, GL_FALSE, &model[0][0]);
            glUniform4f(glGetUniformLocation(shader->program, "color"), i->color.x, i->color.y, i->color.z, i->color.a);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
}

void Particle_generator::update(float dT, Ball& ball)
{
    // spawning...
    for(int i = 0; i < this->max_new_perFrame; ++i)
    {
        respawn_particle(particles[find_free_place()], ball);
    }

    // updating...
    for(std::vector<Particle>::iterator i = particles.begin(); i != particles.end(); ++i)
    {
        i->life -= dT;
        if(i->life > 0)
        {
            i->position += i->velocity * dT; // position update
            i->color.a = i->life/this->full_life; // color update
            i->size += i->size * dT * static_cast<float>(2); // size update
        }
    }
}

void Particle_generator::update(float dT) // tututut
{


    if(fire)
    {
        fire= false;
        // spawning...
        for(int i = 0; i < this->max_new_perFrame; ++i)
        {
            respawn_particle(particles[find_free_place()], *obj);
        }
    }
    // updating...
    for(std::vector<Particle>::iterator i = particles.begin(); i != particles.end(); ++i)
    {
        i->life -= dT;
        if(i->life > 0)
        {
            i->position += i->velocity * dT; // position update
            //i->color.a = i->life/this->full_life; // color update
            //i->size += i->size * dT * static_cast<float>(2); // size update
        }
    }
}

int Particle_generator::find_free_place()
{
    for(unsigned int i = this->last_used_particle; i < this->max_particles; ++i)
    {
        if(particles[i].life <= 0)

            return this->last_used_particle = i;
    }

    for(unsigned int i = 0; i < this->last_used_particle; ++i)
    {
        if(particles[i].life <= 0)

            return this->last_used_particle = i;
    }

    // if all particles active
    return this->last_used_particle = 0;
}

void Particle_generator::respawn_particle(Particle& particle, Ball& ball)
{
    // random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-1, 1);

    particle.color = this->color;
    particle.size = this->size;

    particle.position.x = ball.position.x + ball.radius + dist(mt) * this->spawn_range.x;
    particle.position.y = ball.position.y + ball.radius + dist(mt) * this->spawn_range.y;

    if(ball.isStuck)
        particle.velocity = glm::vec2(0, 0);
    else
        particle.velocity = ball.velocity * static_cast<float>(0.05);

    particle.life = this->full_life;
}

void Particle_generator::respawn_particle(Particle& particle, Block& object) // tututut
{
    // random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-2, 2);

    particle.color = this->color;
    particle.size = this->size;

    particle.position.x = object.position.x + object.size.x/2.0 + dist(mt) * this->spawn_range.x;
    particle.position.y = object.position.y + object.size.y/2.0 + dist(mt) * this->spawn_range.y;

    particle.velocity = glm::vec2(dist(mt)*100, dist(mt)*100);

    particle.life = this->full_life;

}

void Particle_generator::init_draw_fun()
{
    GLfloat vertices[] = {
        -0.5, -0.5, 0, 0,
        0.5, -0.5, 1, 0,
        0.5, 0.5, 1, 1,

        0.5, 0.5, 1, 1,
        -0.5, 0.5, 0, 1,
        -0.5, -0.5, 0, 0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
