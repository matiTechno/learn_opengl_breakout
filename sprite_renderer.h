#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <shader.h>
#include <texture.h>


class Sprite_renderer
{

public:

    Sprite_renderer(Shader* shader);

    ~Sprite_renderer();

    void render(Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec3 color, GLfloat rotation);

private:

    GLuint VAO, VBO;
    Shader* shader;
};

#endif // SPRITE_RENDERER_H
