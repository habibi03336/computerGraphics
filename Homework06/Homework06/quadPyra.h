#include "shader.h"

class QuadPyra {
public:
    // vertex position array
    GLfloat quadPyraVertices[54]  = {
        0.f, .5f, 0.f,  -.5f, -.5f, .5f,  .5f, -.5f, .5f, // a, b, c (front)
        0.f, .5f, 0.f,  .5f, -.5f, .5f,  .5f, -.5f, -.5f, // a, c, d (right)
        0.f, .5f, 0.f,  .5f, -.5f, -.5f,  -.5f, -.5f, -.5f, // a, d, e (back)
        0.f, .5f, 0.f,  -.5f, -.5f, -.5f,  -.5f, -.5f, .5f, // a, e, b (left)
        -.5f, -.5f, .5f,  -.5f, -.5f, -.5f,  .5f, -.5f, -.5f, // b, e, d (bottom1)
        -.5f, -.5f, .5f,  .5f, -.5f, .5f,  .5f, -.5f, -.5f // b, c, d (bottom2)
    };
    
    
    // colour array
    GLfloat quadPyraColors[72] = { // initialized as RGBA sollid color for each face, 96 elements
        1.f, 0.f, 0.f, 1.f,   1.f, 0.f, 0.f, 1.f,   1.f, 0.f, 0.f, 1.f, //  (front)
        0.f, 1.f, 0.f, 1.f,   0.f, 1.f, 0.f, 1.f,   0.f, 1.f, 0.f, 1.f, //  (right)
        0.f, 0.f, 1.f, 1.f,   0.f, 0.f, 1.f, 1.f,   0.f, 0.f, 1.f, 1.f, //  (back)
        1.f, 1.f, 0.f, 1.f,   1.f, 1.f, 0.f, 1.f,   1.f, 1.f, 0.f, 1.f, //  (left)
        1.f, 0.f, 1.f, 1.f,   1.f, 0.f, 1.f, 1.f,   1.f, 0.f, 1.f, 1.f, //  (bottom1)
        1.f, 0.f, 1.f, 1.f,   1.f, 0.f, 1.f, 1.f,   1.f, 0.f, 1.f, 1.f  //  (bottom2)
    };

    unsigned int VAO;
    unsigned int VBO;
    
    unsigned int vSize = sizeof(quadPyraVertices);
    unsigned int cSize = sizeof(quadPyraColors);
    
    QuadPyra() {
        initBuffers();
    };
    
    QuadPyra(float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        initBuffers();
    };
    
    QuadPyra(float dx, float dy, float dz, float s) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    QuadPyra(float dx, float dy, float dz, float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    void initBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize+cSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadPyraVertices);
        glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, quadPyraColors);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) vSize); //color attrib
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };
    
    void draw(Shader *shader) {
        shader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);
    };
    
    void translate(float dx, float dy, float dz) {
        for (int i = 0; i < vSize; i++) {
            if (i % 3 == 0) quadPyraVertices[i] += dx;
            else if (i % 3 == 1) quadPyraVertices[i] += dy;
            else if (i % 3 == 2) quadPyraVertices[i] += dz;
        }
    };
    
    void scale(float s) {
        for (int i = 0; i < vSize; i++)
            quadPyraVertices[i] *= s;
    };
    
    void scale2(float sx, float sy, float sz) {
        for (int i = 0; i < vSize; i++) {
            if (i % 3 == 0) quadPyraVertices[i] *= sx;
            else if (i % 3 == 1) quadPyraVertices[i] *= sy;
            else if (i % 3 == 2) quadPyraVertices[i] *= sz;
        }
    }
};

