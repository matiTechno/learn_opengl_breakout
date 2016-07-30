#include "ball.h"

Ball::Ball(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, float radius,
           const glm::vec2& velocity, const glm::vec3& color, GLfloat rotation, bool isStuck):

    GameObject(sprite_renderer, texture, position, glm::vec2(radius*2, radius*2), color, rotation)
{
    this->velocity = velocity;
    this->radius = radius;
    this->isStuck = isStuck;

    isSticky = false;
    passThrough = false;
}

void Ball::reset(const glm::vec2& velocity)
{
    isStuck = true;
    this->velocity = velocity;
}

void Ball::move(GLfloat dt, int w_width)
{
        position += velocity * dt;

        if(position.x <= 0)
        {
            velocity.x = -velocity.x;
            position.x = 0;
        }
        else if(position.x + size.x >= w_width)
        {
            velocity.x = -velocity.x;
            position.x = w_width - size.x;
        }
        if(position.y <= 0)
        {
            velocity.y = -velocity.y;
            position.y = 0;
        }
}
