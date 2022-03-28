#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle
        GLuint program;

    public:
        void create();
        void destroy();

        //ShaderProgram(){ program = 0; }
        ~ShaderProgram(){ destroy(); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            //Done: call opengl to use the program identified by this->program
            glUseProgram(this->program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //DONE: call opengl to get the uniform location for the uniform defined by name from this->program
            // input : name     , the name of the unifrom variable 
            // return : integer represting the location of the uniform variable
            return glGetUniformLocation(this->program , name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //DONE: call opengl to set the value to the uniform defined by name
            //input : uniform  , the name of the uniform variable
            //        value    , the value to set to the uniform variable
            // the function sets the unform varuable
            glUniform1f(getUniformLocation(uniform) , value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            //input : uniform  , the name of the uniform variable
            //        value    , the value to set to the uniform variable


            // we set the uniform with the values in value vector
            // uniform.x=value.x , uniform.y=value.y
            glUniform2f(getUniformLocation(uniform) , value[0] , value[1]);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            //input : uniform  , the name of the uniform variable
            //        value    , the value to set to the uniform variable


            // we set the uniform with the values in value vector
            // uniform.x=value.x , uniform.y=value.y , uniform.z=value.z 
            glUniform3f(getUniformLocation(uniform) , value[0] , value[1],value[2]);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //DONE: call opengl to set the value to the uniform defined by name

            //input : uniform  , the name of the uniform variable
            //        value    , the value to set to the uniform variable

            // we set the uniform with the values in value vector
            // uniform.x=value.x , uniform.y=value.y , uniform.z=value.z , uniform.w=value.w 
             glUniform4f(getUniformLocation(uniform) , value[0] , value[1],value[2], value[3]);
        }


        //TODO: Delete the copy constructor and assignment operator
        //Question: Why do we do this? Hint: Look at the deconstructor 
        // we need this as if we copied the shader program , and then deleted one of them by the id , opebgl will keep using the other one which will make an error
        
    };

}

#endif