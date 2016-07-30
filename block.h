#ifndef BLOCK_H
#define BLOCK_H

#include <gameobject.h>

class Block: public GameObject
{

public:

    Block(Sprite_renderer* sprite_renderer, Texture* texture, const glm::vec2& position, const glm::vec2& size, bool isSolid,
          const glm::vec3& color = glm::vec3(1, 1, 1), GLfloat rotation = 0, bool isDestroyed = false);

    bool isSolid;
    bool isDestroyed;

    Block * returnBLOCK()
    {
        return this;
    }
};

#endif // BLOCK_H
