#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

// shaders filenames should end with .vs/.fs/.gs
class Shader
{
public:

    Shader(const std::string& fileName_no_ext, bool is_geo = false);

    ~Shader();

    void bind();

    GLuint program;

    Shader(const Shader&); // copy constructor
    Shader& operator=(const Shader&); // assignment operator deletion (not anymore)

private:

    std::string fileName_no_ext;

    bool is_geo;

    GLuint compile(const std::string& data, GLenum shaderType);

    std::string load(const std::string& fileName);

    void printError(GLuint object, bool is_shader, GLenum flag, const std::string& error_msg);

};

#endif // SHADER_H
