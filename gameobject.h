#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <texture.h>
#include <sprite_renderer.h>

class GameObject
{

public:

    GameObject(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, const glm::vec2& size,
               const glm::vec3& color = glm::vec3(1, 1, 1), GLfloat rotation = 0);

    void draw();

    void move(float x, float y);

    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
    GLfloat rotation;

protected:

    Sprite_renderer* sprite_renderer;
    Texture *texture;
};

#endif // GAMEOBJECT_H
