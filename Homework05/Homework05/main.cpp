//
//  main.cpp
//  Homework05
//
//  Created by 하지훈 on 2022/04/05.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shader.h>
#include <cmath>

glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 greenColor = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 yellowColor = glm::vec3(1.0f, 1.0f, 0.0f);
glm::mat4 identityMatrix = glm::mat4(1.0f);
glm::mat4 transform = glm::mat4(1.0f);
float yellowRadius;
float yellowRotation;
float currentTime;

char title[] = "Homework05";
int SCR_WIDTH = 600;
int SCR_HEIGHT = 600;
unsigned int VAO;
unsigned int VBO;

int main(int argc, const char * argv[]) {
    
    //setup
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }
    
    //stuffs about version of openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; glewInit();
    
    //create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // positions that will draw rect by triangle fan.
    float vertices[] = {
        -0.025f, 0.025f, 0.f,
        0.025f, 0.025f, 0.f,
        0.025f, -0.025f, 0.f,
        -0.025f, -0.025f, 0.f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
     
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    Shader myShader("vertexShader.vs", "fragShader.fs");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        myShader.use();
        currentTime = glfwGetTime();
        
        //draw green rect.
        myShader.setVec3("uniformColor", greenColor);
        transform = glm::rotate(identityMatrix, (float)(currentTime * 2 * M_PI), glm::vec3(0.f, 0.f, 1.f));
        myShader.setMat4("transform", transform);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        //draw yellow rect.
        myShader.setVec3("uniformColor", yellowColor);
        //Q1. 왜 튕기는 듯이 보이지?
        //A. Radius의 값을 +로 해야한다고 생각한게 문제였음.
        yellowRadius = sin(currentTime / 3) * 0.5;
        yellowRotation = currentTime / 2 * M_PI;
        transform = glm::rotate(identityMatrix, yellowRotation, glm::vec3(0.f, 0.f, 1.f));
        transform = glm::translate(transform, glm::vec3(yellowRadius, 0.f, 0.f));
        myShader.setMat4("transform", transform);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        //draw red rect.
        myShader.setVec3("uniformColor", redColor);
        //use transform of yellow rect as a baseline.
        transform = glm::rotate(transform, (float)(currentTime * M_PI), glm::vec3(0.f, 0.f, 1.f));
        transform = glm::scale(transform, glm::vec3(1.f, 4.f, 1.f));
        transform = glm::translate(transform, glm::vec3(0.f, 0.025f, 0.f));
        myShader.setMat4("transform", transform);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
         
        glBindVertexArray(0);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}
