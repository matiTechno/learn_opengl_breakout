#include "gameobject.h"

GameObject::GameObject(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, const glm::vec2& size,
                       const glm::vec3& color, GLfloat rotation):
    sprite_renderer(sprite_renderer),
    texture(texture)
{
    this->position = position;
    this->size = size;
    this->color = color;
    this->rotation = rotation;
}

void GameObject::draw()
{
    sprite_renderer->render(texture, position, size, color, rotation);
}

void GameObject::move(float x, float y)
{
    position.x += x;
    position.y += y;
}
