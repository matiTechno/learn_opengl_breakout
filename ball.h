#ifndef BALL_H
#define BALL_H

#include <gameobject.h>
#include <glm/glm.hpp>


class Ball: public GameObject
{

public:

    Ball(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, float radius,
         const glm::vec2& velocity, const glm::vec3& color = glm::vec3(1, 1, 1), GLfloat rotation = 0, bool isStuck = true);

    void reset(const glm::vec2& velocity);

    void move(GLfloat dt, int w_width);

    glm::vec2 velocity;
    float radius;
    bool isStuck;
    bool isSticky;
    bool passThrough;
};

#endif // BALL_H
