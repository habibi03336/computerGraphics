#include "shader.h"



class Quadpyra {
public:
    float imgWidth = 3694.f;
    float imgHeight = 2080.f;
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
    
    GLfloat quadPyraTexCoords_[36] = { // 48 elements
        1906.f, 2076.f,   1586.f, 1424.f,   2236.f, 1421.f,               // (front)
        2888.f, 1102.f,  2236.f, 1421.f,   2236.f, 779.f,               // (right)
        1904.f, 125.f,  2231.f, 779.f,   1589.f, 779.f,                // (back)
        934.f, 1104.f,   1589.f, 779.f,   1586.f, 1424.f,             // (left)
        1586.f, 1424.f,   1589.f, 799.f,   2231.f, 779.f,           // (bottom)
        1586.f, 1424.f,  2236.f, 1421.f,   2231.f, 779.f,                // (bottom)
    };
    
    GLfloat quadPyraTexCoords[36];

    unsigned int VAO;
    unsigned int VBO;
    
    unsigned int vSize = sizeof(quadPyraVertices);
    unsigned int cSize = sizeof(quadPyraColors);
    unsigned int tSize = sizeof(quadPyraTexCoords);
    
    Quadpyra() {
        initBuffers();
    };
    
    Quadpyra(float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        initBuffers();
    };
    
    Quadpyra(float dx, float dy, float dz, float s) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    Quadpyra(float dx, float dy, float dz, float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    void initBuffers() {
        //set ratioCoord
        for(int i = 0; i < 36; i++)
        {
            if (i % 2 == 0) quadPyraTexCoords[i] = ratioCoord(quadPyraTexCoords_[i], true);
            else quadPyraTexCoords[i] = ratioCoord(quadPyraTexCoords_[i], false);
        }

        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize+cSize+tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadPyraVertices);
        glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, quadPyraColors);
        glBufferSubData(GL_ARRAY_BUFFER, vSize+cSize, tSize, quadPyraTexCoords);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) vSize); //color attrib
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+cSize));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
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
    
    GLfloat ratioCoord(GLfloat coord,  bool width)
    {
        if (width) return coord/imgWidth;
        else return coord/imgHeight;
    };

};

