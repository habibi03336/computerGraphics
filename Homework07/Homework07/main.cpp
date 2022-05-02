//
//  main.cpp
//  Homework07
//
//  Created by 하지훈 on 2022/05/01.
//
#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include "quadpyra.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLFWwindow *glAllInit();
GLFWwindow *window = NULL;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void render();
void loadTexture();
void transformationInit();
glm::vec3 getCursorArcPostion(GLFWwindow* window);

char title[] = "Homework07";
unsigned int texture;
int SCR_WIDTH = 600;
int SCR_HEIGHT = 600;
Shader *quadPyraShader;
Quadpyra *quadPyra;
glm::vec3 startCoords;
glm::vec3 currentCoords;
bool drag = false;

glm::mat4 identity = glm::mat4(1.0f);;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;


int main(int argc, const char * argv[]) {
    
    window = glAllInit();
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    quadPyra = new Quadpyra();
    quadPyraShader = new Shader("vshader.vs", "fshader.fs");
    transformationInit();
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    loadTexture();
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}

void render()
{
    quadPyraShader->use();
    glBindTexture(GL_TEXTURE_2D, texture);
    quadPyra->draw(quadPyraShader);
    
    if (drag == true){
        currentCoords = getCursorArcPostion(window);
        float dot = (float)glm::dot(startCoords, currentCoords);
        GLfloat angle = acos((1.0f < dot) ? 1.0f : dot) * 0.1;
        if (angle > 0.0001)
        {
            glm::vec3 camAxis = glm::cross(startCoords, currentCoords);
            view = view * glm::rotate(identity, glm::degrees(angle), camAxis);
            quadPyraShader->setMat4("view", view);
            startCoords = currentCoords;
        }
    }
}

void transformationInit()
{
    model = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    quadPyraShader->use();
    quadPyraShader->setMat4("view", view);
    quadPyraShader->setMat4("projection", projection);
    quadPyraShader->setMat4("model", model);
}

GLFWwindow *glAllInit()
{
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(window);
    
    glEnable(GL_DEPTH_TEST);
    glewExperimental = GL_TRUE; glewInit();
    
    return window;
}

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
    
    char imagePath[] = "eggytexture.png";
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        startCoords = getCursorArcPostion(window);
        drag = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        drag = false;
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        transformationInit();
    }
}

glm::vec3 getCursorArcPostion(GLFWwindow* window){
    double clientX;
    double clientY;
    GLFWAPI::glfwGetCursorPos(window, &clientX, &clientY);
    glm::vec3 coord(0.0f);
    coord.x = clientX/(SCR_WIDTH/2) - 1;
    coord.y = -1 * (clientY/(SCR_HEIGHT/2) - 1);
    
    // bind x,y between -1, 1 for sure.
    coord.x = glm::clamp(coord.x, -1.0f, 1.0f);
    coord.y = glm::clamp(coord.y, -1.0f, 1.0f);
    
    float length_squared = coord.x * coord.x + coord.y * coord.y;
    if (length_squared <= 1.0) coord.z = sqrt(1.0 - length_squared);
    else coord = glm::normalize( coord );
    
    return coord;
}

