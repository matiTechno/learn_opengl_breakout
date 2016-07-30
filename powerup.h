#ifndef POWERUP_H
#define POWERUP_H

#include <gameobject.h>
#include <texture.h>

class PowerUp: public GameObject
{
public:

    PowerUp(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, const glm::vec2& size, const std::string& type,
            float duration, glm::vec2 velocity, const glm::vec3& color = glm::vec3(1, 1, 1), GLfloat = 0):

        GameObject(sprite_renderer, texture, position, size, color, rotation),
        type(type),
        velocity(velocity),
        duration(duration),
        isActive(false),
        isDestroyed(false)
    {

    }

    PowerUp(const PowerUp&) = default;
    PowerUp& operator=(const PowerUp&) = default;

    std::string type;
    glm::vec2 velocity;
    float duration;
    bool isActive;
    bool isDestroyed;

    void move(float dT)
    {
        position += velocity * dT;
    }
};

#endif // POWERUP_H
