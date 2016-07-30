#include "postprocessor.h"
#include <iostream>

PostProcessor::PostProcessor(Shader& shader, int img_width, int img_height):
    shader(shader),
    img_width(img_width),
    img_height(img_height)
{
    shake = false;
    invert = false;
    edge_det = false;

    // create buffers
    glGenFramebuffers(1, &mulitS_FBO);
    glGenFramebuffers(1, &normal_FBO);
    glGenRenderbuffers(1, &RBO);

    // init multiS_FBO with randerBuffer
    glBindFramebuffer(GL_FRAMEBUFFER, mulitS_FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, img_width, img_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "could not complete framebuffer" << std::endl;

    // init normal_FBO with texture
    glBindFramebuffer(GL_FRAMEBUFFER, normal_FBO);
    // texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // attach texture to normal_FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "could not complete framebuffer" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->initBoxMesh();

    // shaders uniforms
    shader.bind();

    float offset = 1.0/300;
    float offsets[9][2] =
    {
        {-offset, offset},
        {0, offset},
        {offset, offset},
        {-offset, 0},
        {0, 0},
        {offset, 0},
        {-offset, -offset},
        {0, -offset},
        {offset, -offset}
    };

    glUniform2fv(glGetUniformLocation(shader.program, "offsets"), 9, &offsets[0][0]);

    int edge_kernel[9] =
    {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(shader.program, "edge_kernel"), 9, edge_kernel);

    float blur_kernel[9] =
    {
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16
    };
    glUniform1fv(glGetUniformLocation(shader.program, "blur_kernel"), 9, blur_kernel);
}

PostProcessor::~PostProcessor()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &texture);

    glDeleteFramebuffers(1, &mulitS_FBO);
    glDeleteFramebuffers(1, &normal_FBO);
    glDeleteRenderbuffers(1, &RBO);
}

void PostProcessor::beginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mulitS_FBO);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mulitS_FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, normal_FBO);
    glBlitFramebuffer(0, 0, img_width, img_height, 0, 0, img_width, img_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::renderEffect(float dT)
{
    shader.bind();
    glUniform1f(glGetUniformLocation(shader.program, "time"), dT);
    glUniform1i(glGetUniformLocation(shader.program, "shake"), shake);
    glUniform1i(glGetUniformLocation(shader.program, "invert"), invert);
    glUniform1i(glGetUniformLocation(shader.program, "edge_det"), edge_det);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::initBoxMesh()
{
    GLfloat vertices[] = {
        -1, -1, 0, 0,
        1, -1, 1, 0,
        1, 1, 1, 1,

        1, 1, 1, 1,
        -1, 1, 0, 1,
        -1, -1, 0, 0
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
