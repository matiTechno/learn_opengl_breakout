#include "block.h"

Block::Block(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, const glm::vec2& size, bool isSolid,
             const glm::vec3& color, GLfloat rotation, bool isDestroyed):

    GameObject(sprite_renderer, texture, position, size, color, rotation)
{
    this->isSolid = isSolid;
    this->isDestroyed = isDestroyed;
}
