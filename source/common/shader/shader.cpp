#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

void our::ShaderProgram::create() {
    //Create Shader Program
    program = glCreateProgram();
}

void our::ShaderProgram::destroy() {
    //Delete Shader Program from gpu and reference is deleted from cpu
    if(program != 0) glDeleteProgram(program);
    program = 0;
}

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {

    // input : filename  , the name of the shader txt file
    //         type      , the type of the shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)

    // Here, we open the file and read a string from it containing the GLSL code of our shader
    // Open shader file
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    // Convert file to string
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    // OpenGL handles char only not string
    const char* sourceCStr = sourceString.c_str();
    file.close();

    // calling glCreateShader to allocate space in gpu for the shader
    GLuint shaderID = glCreateShader(type);

    //DONE: send the source code to the shader and compile it
    
    // Number of sources codes and array to them
    // send the source code of shader object and compile it 
    // input:   shader: shader object whose source code is to be replaced.
    //          count: number of elements in the string and length arrays.
    //          string: array of pointers to strings containing the source code to be loaded into the shader.
    //          length: array of string lengths.
    glShaderSource(shaderID , 1 ,&sourceCStr , nullptr);
    glCompileShader(shaderID);
    
    // Here we check for compilation errors
    //DONE: Uncomment this if block
    // to check for compilation errors and dealocate shader from gpu if any.
    if(std::string error = checkForShaderCompilationErrors(shaderID); error.size() != 0){
        std::cerr << "ERROR IN " << filename << std::endl;
        std::cerr << error << std::endl;
        glDeleteShader(shaderID);
        return false;
    }

    
    //DONE: attach the shader to the program then delete the shader
    glAttachShader(program , shaderID);
    // After ataching the shader no need to save it
    glDeleteShader(shaderID);
    //We return true since the compilation succeeded
    return true;
}



bool our::ShaderProgram::link() const {
    // function that links the shader program
    //DONE: call opengl to link the program identified by this->program 
    // Assembles frog shader, Vertex shader into a single program
     glLinkProgram(this->program);
    // Here we check for linking errors
    //DONE: Uncomment this if block
    if(auto error = checkForLinkingErrors(program); error.size() != 0){
        std::cerr << "LINKING ERROR" << std::endl;
        std::cerr << error << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}