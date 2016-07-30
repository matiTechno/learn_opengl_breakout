#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>


class Texture
{

public:

    Texture(const std::string& fileName);

    ~Texture();

    void bind(GLuint unit = 0);

    Texture(const Texture&); // copy constructor
    Texture& operator=(const Texture&); // assignment operator deletion (not any more)

private:

    GLuint texture;

    std::string fileName;
};

#endif // TEXTURE_H
