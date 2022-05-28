#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //DONE: (Req 1) Write this function

            // Create a vertex array object (VAO)
            glGenVertexArrays(1, &VAO);
            // Bind the VAO
            glBindVertexArray(VAO);
            
            // Create a vertex buffer object (VBO)
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION); // Enable the position attribute
            // Tell the vertex attribute how to get the position data out of the vertex buffer
            glVertexAttribPointer( // Define the position attribute
                ATTRIB_LOC_POSITION, // Location
                3, // Number of components
                GL_FLOAT, // Type
                GL_FALSE, // Normalized
                sizeof(Vertex), // Stride
                (void*)offsetof(Vertex, position) // Offset
            );
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);   // Enable the color attribute
            glVertexAttribPointer( // Define the color attribute
                ATTRIB_LOC_COLOR, // Location
                4, // Number of components
                GL_UNSIGNED_BYTE, // Type
                GL_TRUE, // Normalized
                sizeof(Vertex), // Stride
                (void*)offsetof(Vertex, color) // Offset
            );
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD); // Enable the texture coordinate attribute
            glVertexAttribPointer( // Define the texture coordinate attribute
                ATTRIB_LOC_TEXCOORD, // Location
                2, // Number of components
                GL_FLOAT, // Type
                GL_FALSE, // Normalized
                sizeof(Vertex), // Stride
                (void*)offsetof(Vertex, tex_coord) // Offset
            );
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);  // Enable the normal attribute
            glVertexAttribPointer( // Define the normal attribute
                ATTRIB_LOC_NORMAL, // Location
                3, // Number of components
                GL_FLOAT, // Type
                GL_FALSE, // Normalized
                sizeof(Vertex), // Stride
                (void*)offsetof(Vertex, normal) // Offset
            );
            // Create an element buffer object (EBO)
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);

            // remember to store the number of elements in "elementCount" since you will need it for drawing
            elementCount = (GLsizei)elements.size();
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
        }

        // this function should render the mesh
        void draw() 
        {
            //DONE: (Req 1) Write this function
            glBindVertexArray(VAO);
            glDrawElements(
                GL_TRIANGLES, // mode
                elementCount,  // count
                GL_UNSIGNED_INT, // type
             0 // element array buffer offset
             );
            
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //DONE: (Req 1) Write this function
            // Delete the VAO
            glDeleteVertexArrays(1, &VAO);
            // Delete the VBO
            glDeleteBuffers(1, &VBO);
            // Delete the EBO
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}