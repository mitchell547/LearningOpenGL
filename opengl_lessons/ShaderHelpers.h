
#ifndef SHADERHELPERS_H
#define SHADERHELPERS_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Shader
{
private:
    GLuint _shaderID = 0;
    GLenum _shaderType;
public:
    Shader(const std::string& sourcePath, GLenum shaderType) : _shaderType(shaderType)
    {
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::stringstream buffer;
        std::string shaderCode;
        try
        {
            shaderFile.open(sourcePath);            
            buffer << shaderFile.rdbuf();
            shaderFile.close();
            shaderCode = buffer.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "Can't read shader file " << sourcePath << ": " << e.what() << std::endl;
            throw;
        }
        const char* shaderSource = shaderCode.c_str();

        _shaderID = glCreateShader(shaderType);
        glShaderSource(_shaderID, 1, &shaderSource, NULL);
        glCompileShader(_shaderID);
        _checkShader();
    }

    ~Shader()
    {
        glDeleteShader(_shaderID);
    }

    GLuint getID() { return _shaderID; }

private:
    void _checkShader() {
        int  success;
        char infoLog[512];
        glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(_shaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            throw std::runtime_error("Shader compilation failed");            
        }
    }
};

class ShaderProgram
{
private:
    GLuint _programID = 0;

public:
    ShaderProgram(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
    {
        Shader vertexShader(vertexShaderSrc, GL_VERTEX_SHADER);
        Shader fragmentShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);

        _programID = glCreateProgram();
        glAttachShader(_programID, vertexShader.getID());
        glAttachShader(_programID, fragmentShader.getID());
        glLinkProgram(_programID);
        _checkProgram(_programID);
    }

    GLuint getID() { return _programID; }

private:
    void _checkProgram(GLuint program) {
        int  success;
        char infoLog[512];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
            throw std::runtime_error("Shader program compilation failed");
        }
    }
};

#endif // !SHADERHELPERS_H
