//
//  main.cpp
//  Homework04
//
//  Created by 하지훈 on 2022/03/30.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#include <cmath>

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
float quadCalculator(float x);
void getCursorRelativePostion(float *arr, GLFWwindow* window);
void getInterCoords(float *result, float *coord1, float *coord2, float a, float b, float c);


glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 greenColor = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 yellowColor = glm::vec3(1.0f, 1.0f, 0.0f);
int INVISIBLE = 100;
int SCR_WIDTH = 600;
int SCR_HEIGHT = 600;
int NumOfSegments = 64;
float startCoords[2];
float currentCoords[2];
float interCoords[4];
float coeffA = -5.f;
float coeffB = - (4 / 5.f);
float coeffC = - 0.42f;
bool drag = false;
bool dragDone = false;
unsigned int VAOs[3];
unsigned int VBOs[3];

int main(int argc, const char * argv[]) {
    
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
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework04", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; glewInit();
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    
    // VBO for quadratic curve
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    float vertices[NumOfSegments*3];
    for (int i = 0; i < NumOfSegments*3; i += 3){
        float x = -1 + float(i)/NumOfSegments;
        vertices[i] = x;
        vertices[i+1] = quadCalculator(x);
        vertices[i+2] = 0.f;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
     
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //make shader program.
    Shader myShader("vertexShader.vs", "fragShader.fs");
    
    //set background color.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        myShader.use();
        // make VBO and draw line being dragged.
        if (drag == true && dragDone == false){
            getCursorRelativePostion(currentCoords, window);
            glBindVertexArray(VAOs[1]);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
            float vertices[] = {
                startCoords[0], startCoords[1], 0.f,
                currentCoords[0], currentCoords[1], 0.f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
            glEnableVertexAttribArray(0);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // activate once when drag is done.
        if (drag == true && dragDone == true){
            glBindVertexArray(VAOs[2]);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
            getInterCoords(interCoords, startCoords, currentCoords, coeffA, coeffB, coeffC);
            float vertices[] = {
                interCoords[0], interCoords[1], 0.f,
                interCoords[2], interCoords[3], 0.f,
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
            glEnableVertexAttribArray(0);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            drag = false;
        }
        //draw qaudratic curve
        myShader.setVec3("uniformColor", redColor);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_LINE_STRIP, 0, NumOfSegments);
        //draw line segment
        myShader.setVec3("uniformColor", greenColor);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_LINES, 0, 2);
        //draw intersection point when drag has been done.
        if(dragDone == true){
            myShader.setVec3("uniformColor", yellowColor);
            glPointSize(15.0f);
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_POINTS, 0, 2);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
    
}


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        getCursorRelativePostion(startCoords, window);
        drag = true;
        dragDone = false;
    } else if (drag == true && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        dragDone = true;
    }
};

float quadCalculator(float x){
    float y = coeffA*(x*x) + coeffB*x + coeffC;
    return y;
}

void getCursorRelativePostion(float *arr, GLFWwindow* window){
    double clientX;
    double clientY;
    GLFWAPI::glfwGetCursorPos(window, &clientX, &clientY);
    arr[0] = clientX/(SCR_WIDTH/2) - 1;
    arr[1] = -1 * (clientY/(SCR_HEIGHT/2) - 1);
}

void getInterCoords(float *result, float *coord1, float *coord2, float a, float b, float c){
    float tA = a * ((coord1[0] - coord2[0]) * (coord1[0] - coord2[0])) + FLT_EPSILON;
    float tB = 2 * a * coord1[0] * (coord2[0] - coord1[0]) + b * (coord2[0] - coord1[0]) + coord1[1] - coord2[1];
    float tC = coord1[0] * (b + a * coord1[0]) + c - coord1[1];
    
    float det = tB*tB - 4*tA*tC;
    
    float t_plus = 0.f;
    float t_minus = 0.f;
    if (det >= 0) {
        t_plus = (-tB + sqrt(det))/(2.f*tA);
        t_minus = (-tB - sqrt(det))/(2.f*tA);
    }
    
    if (det >=0){
        result[0] = coord1[0] + (coord2[0] - coord1[0])*t_minus;
        result[1] = coord1[1] + (coord2[1] - coord1[1])*t_minus;
        result[2] = coord1[0] + (coord2[0] - coord1[0])*t_plus;
        result[3] = coord1[1] + (coord2[1] - coord1[1])*t_plus;
    } else {
        result[0] = INVISIBLE;
        result[1] = INVISIBLE;
        result[2] = INVISIBLE;
        result[3] = INVISIBLE;
    }
    
    if (det >= 0) {
        if (t_minus < 0 || t_minus > 1){
            result[0] = INVISIBLE;
            result[1] = INVISIBLE;
        }
        if (t_plus < 0 || t_plus > 1){
            result[2] = INVISIBLE;
            result[3] = INVISIBLE;
        }
    }
}
