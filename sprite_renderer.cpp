#include "sprite_renderer.h"
#include <glm/gtx/transform.hpp>

Sprite_renderer::Sprite_renderer(Shader* shader):
    shader(shader)
{
    GLfloat vertices[] = {
        0, 0, 0, 0,
        1, 0, 1, 0,
        1, 1, 1, 1,

        1, 1, 1, 1,
        0, 1, 0, 1,
        0, 0, 0, 0
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

Sprite_renderer::~Sprite_renderer()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Sprite_renderer::render(Texture* texture, const glm::vec2& position, const glm::vec2& size,
                             const glm::vec3 color, GLfloat rotation)
{
    texture->bind();

    glm::mat4 model;
    // translation
    model = glm::translate(model, glm::vec3(position, 0));
    // rotation
    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0));
    model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0));
    //scaling
    model = glm::scale(model, glm::vec3(size, 1));

    shader->bind();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform3f(glGetUniformLocation(shader->program, "color"), color.x, color.y, color.z);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
