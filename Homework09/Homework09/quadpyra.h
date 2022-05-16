#include "shader.h"
#include<cmath>

class Quadpyra {
public:
    bool FLAT_SHADE = true;
    bool SMOOTH_SHADE = false;
    float sqrt_5 = sqrt(5.f);
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

    GLfloat quadPyraFlatNormals[54] = {
        0.f, 1.f, 2.f,  0.f, 1.f, 2.f,  0.f, 1.f, 2.f, // front
        2.f, 1.f, 0.f,  2.f, 1.f, 0.f,  2.f, 1.f, 0.f, // right
        0.f, 1.f, -2.f,  0.f, 1.f, -2.f,  0.f, 1.f, -2.f, // back
        -2.f, 1.f, 0.f,  -2.f, 1.f, 0.f,  -2.f, 1.f, 0.f, // left
        0.f, -1.f, 0.f,  0.f, -1.f, 0.f,  0.f, -1.f, 0.f, //bottom
        0.f, -1.f, 0.f,  0.f, -1.f, 0.f,  0.f, -1.f, 0.f, //bottom
    };
    
    GLfloat _quadPyraSmoothNormals[15];
    
    GLfloat quadPyraSmoothNormals[54];
    
    unsigned int VAO;
    unsigned int VBO;
    
    unsigned int vSize = sizeof(quadPyraVertices);
    unsigned int cSize = sizeof(quadPyraColors);
    unsigned int tSize = sizeof(quadPyraTexCoords);
    unsigned int nSize = sizeof(quadPyraFlatNormals);
    
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
        
        
        //texture VBO data genertaion
        for(int i = 0; i < 36; i++)
        {
            if (i % 2 == 0) quadPyraTexCoords[i] = ratioCoord(quadPyraTexCoords_[i], true);
            else quadPyraTexCoords[i] = ratioCoord(quadPyraTexCoords_[i], false);
        }
        
        //normal VBO data generation
        for(int i = 0; i < 36; i++)
        {
            quadPyraFlatNormals[i] = quadPyraFlatNormals[i] / sqrt_5;
        }
        
        for(int i = 0; i < 4; i++)
        {
            // 밑면 삼각형 두 개와 닿는 edge
            int j = 0;
            i % 2 == 0 ? j = 2 : j = 1;
            
            _quadPyraSmoothNormals[3*i] = quadPyraFlatNormals[36] * j;
            _quadPyraSmoothNormals[3*i+1] = quadPyraFlatNormals[37] * j;
            _quadPyraSmoothNormals[3*i+2] = quadPyraFlatNormals[38] * j;
        }
        _quadPyraSmoothNormals[0] += quadPyraFlatNormals[0] + quadPyraFlatNormals[27];
        _quadPyraSmoothNormals[1] += quadPyraFlatNormals[1] + quadPyraFlatNormals[28];
        _quadPyraSmoothNormals[2] += quadPyraFlatNormals[2] + quadPyraFlatNormals[29];
        
        _quadPyraSmoothNormals[3] += quadPyraFlatNormals[0] + quadPyraFlatNormals[9];
        _quadPyraSmoothNormals[4] += quadPyraFlatNormals[1] + quadPyraFlatNormals[10];
        _quadPyraSmoothNormals[5] += quadPyraFlatNormals[2] + quadPyraFlatNormals[11];
        
        _quadPyraSmoothNormals[6] += quadPyraFlatNormals[9] + quadPyraFlatNormals[18];
        _quadPyraSmoothNormals[7] += quadPyraFlatNormals[10] + quadPyraFlatNormals[19];
        _quadPyraSmoothNormals[8] += quadPyraFlatNormals[11] + quadPyraFlatNormals[20];
        
        _quadPyraSmoothNormals[9] += quadPyraFlatNormals[18] + quadPyraFlatNormals[27];
        _quadPyraSmoothNormals[10] += quadPyraFlatNormals[19] + quadPyraFlatNormals[28];
        _quadPyraSmoothNormals[11] += quadPyraFlatNormals[20] + quadPyraFlatNormals[29];
        
        _quadPyraSmoothNormals[12] = 0.f;
        _quadPyraSmoothNormals[13] = 1.f;
        _quadPyraSmoothNormals[14] = 0.f;
    
        
        for(int i = 0; i < 18; i++)
        {
            if (i == 0 || i == 3 || i == 6 || i == 9)
            {
                quadPyraSmoothNormals[i*3] = _quadPyraSmoothNormals[12];
                quadPyraSmoothNormals[i*3+1] = _quadPyraSmoothNormals[13];
                quadPyraSmoothNormals[i*3+2] = _quadPyraSmoothNormals[14];
            } else if (i == 1 || i == 11 || i == 12 || i == 15)
            {
                quadPyraSmoothNormals[i*3] = _quadPyraSmoothNormals[0];
                quadPyraSmoothNormals[i*3+1] = _quadPyraSmoothNormals[1];
                quadPyraSmoothNormals[i*3+2] = _quadPyraSmoothNormals[2];
            } else if (i == 2 || i == 4 ||  i == 16)
            {
                quadPyraSmoothNormals[i*3] = _quadPyraSmoothNormals[3];
                quadPyraSmoothNormals[i*3+1] = _quadPyraSmoothNormals[4];
                quadPyraSmoothNormals[i*3+2] = _quadPyraSmoothNormals[5];
            } else if (i == 5 || i == 7 || i == 14 || i == 17)
            {
                quadPyraSmoothNormals[i*3] = _quadPyraSmoothNormals[6];
                quadPyraSmoothNormals[i*3+1] = _quadPyraSmoothNormals[7];
                quadPyraSmoothNormals[i*3+2] = _quadPyraSmoothNormals[8];
            } else if (i == 8 || i == 10 || i == 13)
            {
                quadPyraSmoothNormals[i*3] = _quadPyraSmoothNormals[9];
                quadPyraSmoothNormals[i*3+1] = _quadPyraSmoothNormals[10];
                quadPyraSmoothNormals[i*3+2] = _quadPyraSmoothNormals[11];
            }
                
        }

        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize+cSize+tSize+nSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadPyraVertices);
        glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, quadPyraColors);
        glBufferSubData(GL_ARRAY_BUFFER, vSize+cSize, tSize, quadPyraTexCoords);
        glBufferSubData(GL_ARRAY_BUFFER, vSize+cSize+tSize, nSize, quadPyraFlatNormals);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) vSize);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+cSize));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(vSize+cSize+tSize));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };
    
    void swapShadeMode(bool shadeMode)
    {
        if (shadeMode == FLAT_SHADE)
        {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, vSize+cSize+tSize, nSize, quadPyraFlatNormals);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(vSize+cSize+tSize));
            glEnableVertexAttribArray(3);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        } else if (shadeMode == SMOOTH_SHADE)
        {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, vSize+cSize+tSize, nSize, quadPyraSmoothNormals);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(vSize+cSize+tSize));
            glEnableVertexAttribArray(3);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }
    
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

