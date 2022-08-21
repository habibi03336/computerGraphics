//
//  main.cpp
//  Homework03
//
//  Created by 하지훈 on 2022/03/22.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_cusor_callback(GLFWwindow* window, double xpos, double ypos);

glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 greenColor = glm::vec3(0.0f, 1.0f, 0.0f);

unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

double clientX = SCR_WIDTH/2;
double clientY = SCR_HEIGHT/2;
bool isRed = true;
bool movePosition = false;

int main(int argc, const char * argv[]) {
    
    glfwInit();
    
    //stuffs about version of openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework03", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; glewInit();
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_cusor_callback);
    glfwSetKeyCallback(window, key_callback);
    
    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float vertices[] = {
        -0.1f, 0.1f, 0.f,
        0.1f, 0.1f, 0.f,
        0.1f, -0.1f, 0.f,
        -0.1f, -0.1f, 0.f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader myShader("./Homework03/vertexShader", "./Homework03/fragShader");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    float relativeX, relativeY;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        myShader.use();
        if(isRed)
        {
            myShader.setVec3("uniformColor", redColor);
        } else
        {
            myShader.setVec3("uniformColor", greenColor);
        }
        if(movePosition)
        {
            movePosition = false;
            relativeX = (clientX - 300)/300;
            relativeY = (300 - clientY)/300;
            myShader.setVec2("centerPosition", relativeX, relativeY);
        }
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    // insert code here...
    return 0;
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        isRed = !isRed;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        movePosition = true;
    }
};

void mouse_cusor_callback(GLFWwindow* window, double xpos, double ypos)
{
    clientX = xpos;
    clientY = ypos;
}
