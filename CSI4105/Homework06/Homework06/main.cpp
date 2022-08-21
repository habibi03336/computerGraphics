//
//  main.cpp
//  Homework06
//
//  Created by 하지훈 on 2022/04/12.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include "quadPyra.h"
#include <cmath>

GLFWwindow *glAllInit();
GLFWwindow *window = NULL;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void render();
void transformationInit();
glm::vec3 getCursorArcPostion(GLFWwindow* window);

char title[] = "Homework06";
int SCR_WIDTH = 600;
int SCR_HEIGHT = 600;
Shader *quadPyraShader;
QuadPyra *quadPyra;
glm::vec3 startCoords;
glm::vec3 currentCoords;
bool drag = false;
bool camMode = false;
float currentTime = 0.f;
float priorTime = 0.f;

glm::mat4 identity = glm::mat4(1.0f);;
glm::vec4 cop;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;


int main(int argc, const char * argv[]) {
    
    window = glAllInit();
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    quadPyra = new QuadPyra();
    quadPyraShader = new Shader("vshader.vs", "fshader.fs");
    transformationInit();
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    
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
    quadPyra->draw(quadPyraShader);
    
    if (drag == true && camMode == false){
        currentCoords = getCursorArcPostion(window);
        float dot = (float)glm::dot(startCoords, currentCoords);
        GLfloat angle = acos((1.0f < dot) ? 1.0f : dot) * 0.1;
        if (angle > 0.0001)
        {
            glm::vec3 camAxis = glm::cross(startCoords, currentCoords);
            cop = glm::rotate(identity, -glm::degrees(angle), camAxis) * cop;
            view = view * glm::rotate(identity, glm::degrees(angle), camAxis);
            quadPyraShader->setMat4("view", view);
            startCoords = currentCoords;
        }
    }
    
    if (camMode == true)
    {
        currentTime = glfwGetTime();
        cop = glm::rotate(identity, (float) currentTime - priorTime, glm::vec3(0.f, 3.f, 0.f)) * cop;
        view = glm::lookAt(glm::vec3(cop.x, cop.y, cop.z), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        quadPyraShader->setMat4("view", view);
        priorTime = currentTime;
    }
}

void transformationInit()
{
    cop = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);
    model = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(cop.x, cop.y, cop.z), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
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
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        camMode = !camMode;
        if (camMode == true)
        {
            priorTime = glfwGetTime();
            glm::vec3 a = glm::normalize(glm::vec3(cop.x, 0.f, cop.z));
            cop = glm::vec4(5 * a.x, 3.0f, 5 * a.z, 1.0f);
        }
        else
        {
            transformationInit();
        }
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
