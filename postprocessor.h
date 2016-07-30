#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <GL/glew.h>
#include <shader.h>

// 1 to set effect

class PostProcessor
{

public:

    PostProcessor(Shader& shader, int img_width, int img_height);

    ~PostProcessor();

    void beginRender();

    void endRender();

    void renderEffect(float dT);

    bool shake, invert, edge_det;

private:

    Shader& shader;
    GLuint texture;

    int img_width, img_height;

    GLuint mulitS_FBO, normal_FBO;
    GLuint RBO;
    GLuint VAO, VBO;

    // functions

    void initBoxMesh();
};

#endif // POSTPROCESSOR_H
