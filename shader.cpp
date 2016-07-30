#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string& fileName_no_ext, bool is_geo):

    fileName_no_ext(fileName_no_ext),
    is_geo(is_geo)
{
    program = glCreateProgram();

    GLuint vertexShader = compile(load(fileName_no_ext + ".vs"), GL_VERTEX_SHADER);
    printError(vertexShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".vs");

    GLuint fragmentShader = compile(load(fileName_no_ext + ".fs"), GL_FRAGMENT_SHADER);
    printError(fragmentShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".fs");

    GLuint geometryShader {};
    if(is_geo)
    {
        geometryShader = compile(load(fileName_no_ext + ".gs"), GL_GEOMETRY_SHADER);
        printError(geometryShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".gs");
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if(is_geo)
        glAttachShader(program, geometryShader);

    glLinkProgram(program);

    printError(program, false, GL_LINK_STATUS, "could not link program");

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    if(is_geo)
    {
        glDetachShader(program, geometryShader);
        glDeleteShader(geometryShader);
    }
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

void Shader::bind()
{
    glUseProgram(program);
}

GLuint Shader::compile(const std::string& data, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    const GLchar* string_arr[] = {data.c_str()};
    GLint lenght_arr[] = {static_cast<GLint>(data.length())};

    glShaderSource(shader, 1, string_arr, lenght_arr);
    glCompileShader(shader);

    return shader;
}

std::string Shader::load(const std::string& fileName)
{
    std::string data;
    std::ifstream file(fileName);
    if(file)
    {
        std::stringstream sstream;
        sstream << file.rdbuf();
        data = sstream.str();
    }
    else
        std::cout << "could not open " << fileName << std::endl;
    return data;
}

void Shader::printError(GLuint object, bool is_shader, GLenum flag, const std::string& error_msg)
{
    GLint success = 0;
    if(is_shader)
        glGetShaderiv(object, flag, &success);
    else
        glGetProgramiv(object, flag, &success);

    if(success == GL_FALSE)
    {
        GLint maxLength = 0;

        if(is_shader)
        {
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(object, maxLength, 0, &infoLog[0]);
            std::cout << error_msg << std::endl;
            for (std::vector<GLchar>::const_iterator i = infoLog.begin(); i != infoLog.end(); ++i)
                std::cout << *i;
            std::cout << std::endl;

            glDeleteShader(object);
        }
        else
        {
            glGetProgramiv(object, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(object, maxLength, 0, &infoLog[0]);
            for (std::vector<GLchar>::const_iterator i = infoLog.begin(); i != infoLog.end(); ++i)
                std::cout << *i;
            std::cout << std::endl;

            glDeleteProgram(object);
        }
    }
}

Shader::Shader(const Shader& temp)
{
    fileName_no_ext = temp.fileName_no_ext;
    is_geo = temp.is_geo;

    program = glCreateProgram();

    GLuint vertexShader = compile(load(fileName_no_ext + ".vs"), GL_VERTEX_SHADER);
    printError(vertexShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".vs");

    GLuint fragmentShader = compile(load(fileName_no_ext + ".fs"), GL_FRAGMENT_SHADER);
    printError(fragmentShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".fs");

    GLuint geometryShader {};
    if(is_geo)
    {
        geometryShader = compile(load(fileName_no_ext + ".gs"), GL_GEOMETRY_SHADER);
        printError(geometryShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".gs");
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if(is_geo)
        glAttachShader(program, geometryShader);

    glLinkProgram(program);

    printError(program, false, GL_LINK_STATUS, "could not link program");

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    if(is_geo)
    {
        glDetachShader(program, geometryShader);
        glDeleteShader(geometryShader);
    }
}

Shader& Shader::operator=(const Shader& temp)
{
    if(&temp == this)
        return *this;

    // copy
    fileName_no_ext = temp.fileName_no_ext;
    is_geo = temp.is_geo;

    program = glCreateProgram();

    GLuint vertexShader = compile(load(fileName_no_ext + ".vs"), GL_VERTEX_SHADER);
    printError(vertexShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".vs");

    GLuint fragmentShader = compile(load(fileName_no_ext + ".fs"), GL_FRAGMENT_SHADER);
    printError(fragmentShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".fs");

    GLuint geometryShader {};
    if(is_geo)
    {
        geometryShader = compile(load(fileName_no_ext + ".gs"), GL_GEOMETRY_SHADER);
        printError(geometryShader, true, GL_COMPILE_STATUS, "could not compile " + fileName_no_ext + ".gs");
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if(is_geo)
        glAttachShader(program, geometryShader);

    glLinkProgram(program);

    printError(program, false, GL_LINK_STATUS, "could not link program");

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    if(is_geo)
    {
        glDetachShader(program, geometryShader);
        glDeleteShader(geometryShader);
    }

    return *this;
}
