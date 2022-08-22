#include "../inc/Shader.h"
#include "magic_enum.hpp"
#include <filesystem>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
    hasInit = false;
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::init()
{
    if (glGetError() == GL_INVALID_OPERATION)
    {
        std::cout << "The OpenGL context was not created";
        throw "The OpenGL context was not created";
    }
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexFileName = std::filesystem::path(vertexPath).filename().string();
    std::string fragmentFileName = std::filesystem::path(fragmentPath).filename().string();
    std::string vertexCode_s, fragmentCode_s;
    std::ifstream vertexFile, fragmentFile;
    // ensure ifstream objects can throw exceptions:
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // read file
    try
    {
        // open files
        vertexFile.open(vertexPath);
        // read file's buffer contents into streams
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        // close file
        vertexFile.close();
        // convert stream into string
        vertexCode_s = vertexStream.str();
        if (vertexCode_s.length() > 0)
        {
            std::cout << "Shader file: " << vertexFileName << " Successfully read" << std::endl;
        }
        else
        {
            std::cout << "Shader file: " << vertexFileName << " Read fail" << std::endl;
        }
    }
    catch (const std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_READ_FAILURE in file " << vertexFileName << " : " << e.what() << std::endl;
    }

    try
    {
        // open files
        fragmentFile.open(fragmentPath);
        // read file's buffer contents into streams
        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        // close file
        fragmentFile.close();
        // convert stream into string
        fragmentCode_s = fragmentStream.str();
        if (fragmentCode_s.length() > 0)
        {
            std::cout << "Shader file: " << fragmentFileName << " Successfully read" << std::endl;
        }
        else
        {
            std::cout << "Shader file: " << fragmentFileName << " Read fail" << std::endl;
        }
    }
    catch (const std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_READ_FAILURE in file " << fragmentFileName << " : " << e.what() << std::endl;
    }

    // 2. compile shaders

    const char *vertexCode_c = vertexCode_s.c_str();
    const char *fragmentCode_c = fragmentCode_s.c_str();

    // vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode_c, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, CompileType::VERTEX);

    // fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode_c, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, CompileType::FRAGMENT);

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, CompileType::PROGRAM);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    hasInit = true;
}

void Shader::checkCompileErrors(unsigned int shader, CompileType type)
{
    int success;
    char infoLog[1024];
    if (type != CompileType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
                << magic_enum::enum_name(type) << std::endl
                << infoLog << std::endl
                << LONG_LINE << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
                << magic_enum::enum_name(type) << std::endl
                << infoLog << std::endl
                << LONG_LINE << std::endl;
        }
    }
}

void Shader::use() const
{
    if (hasInit)
    {
        glUseProgram(ID);
    }
    else
    {
        std::cout << "Shader has not been initialized";
        throw "Shader has not been initialized";
    }
}

void Shader::setBool(const std::string &name, const bool &value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, const int &value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setFloat(const std::string &name, const float &value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloatArray(const std::string &firstElementName, const int &index, const float &value) const
{
    std::string secondElementName = firstElementName;
    int indexPos = firstElementName.find("[0]");
    if (indexPos == std::string::npos)
        std::cout << "NOT FIRST ELEMENT NAME";
    else
    {
        secondElementName.replace(indexPos, 3, "[1]");
        int elementSize = glGetUniformLocation(ID, secondElementName.c_str()) - glGetUniformLocation(ID, firstElementName.c_str());
        glUniform1f(glGetUniformLocation(ID, firstElementName.c_str()) + index * elementSize, value);
    }
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec3(const std::string &name, const float &x, const float &y, const float &z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3Array(const std::string &firstElementName, const int &elementSize, const int &index, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(ID, firstElementName.c_str()) + index * elementSize, 1, glm::value_ptr(vec));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const std::string &name, const float &x, const float &y, const float &z, const float &w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
