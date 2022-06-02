//
//  ground.h
//  final_project
//
//  Created by 하지훈 on 2022/05/31.
//
#include <shader.h>

#ifndef ground_h
#define ground_h
class Background {
private:
    float vertices[18];
    float normals[18];
    float texCoords[12];
    constexpr static int vSize = sizeof(vertices);
    constexpr static int nSize = sizeof(normals);
    constexpr static int tSize = sizeof(texCoords);

    unsigned int VAO;
    unsigned int VBO;
    unsigned int texture;
    
    char *imagePath;
    
    
    void loadTexture() {
        
        // Create texture ids.
        glGenTextures(1, &texture);
        
        // All upcomming GL_TEXTURE_2D operations now on "texture" object
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters for wrapping.
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // Set texture parameters for filtering.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        int width, height, nrChannels;
        unsigned char *image = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (!image) {
            printf("texture %s loading error ... \n", imagePath);
        }
        else printf("texture %s loaded\n", imagePath);
        
        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    
    void initBuffers() {
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize+nSize+tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, normals);
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize, tSize, texCoords);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) vSize); //normal attrib
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+nSize));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        loadTexture();
    };
    
    
public:
    Background(float repeat, char *imgPath)
    {
        for(int i = 0; i < 6; i++)
        {
            normals[i*3] = 0.f;
            normals[i*3+1] = 1.f;
            normals[i*3+2] = 0.f;
        }
        
        texCoords[0] = 0.f;
        texCoords[1] = 0.f;
        texCoords[2] = 0.f;
        texCoords[3] = repeat;
        texCoords[4] = repeat;
        texCoords[5] = repeat;
        texCoords[6] = 0.f;
        texCoords[7] = 0.f;
        texCoords[8] = repeat;
        texCoords[9] = 0.f;
        texCoords[10] = repeat;
        texCoords[11] = repeat;
        
        imagePath = imgPath;
        vertices[0] = - 1;
        vertices[1] = 0.f;
        vertices[2] = - 1;
        
        vertices[3] = 1;
        vertices[4] = 0.f;
        vertices[5] = - 1;
        
        vertices[6] = 1;
        vertices[7] = 0.f;
        vertices[8] = 1;
        
        vertices[9] = - 1;
        vertices[10] = 0.f;
        vertices[11] = - 1;
        
        vertices[12] = - 1;
        vertices[13] = 0.f;
        vertices[14] = 1;
        
        vertices[15] = 1;
        vertices[16] = 0.f;
        vertices[17] = 1;
        
        initBuffers();
    }
    
    
    void Draw(Shader *shader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        shader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
};

#endif /* ground_h */
