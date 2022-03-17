//
//  main.cpp
//  Homework02
//
//  Created by 하지훈 on 2022/03/15.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow *window);
void press_p_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

const char *vertextShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *greenShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.f, 1.f, 0.f, 1.0f);\n"
    "}\n\0";

const char *redShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.f, 0.f, 0.f, 1.0f);\n"
    "}\n\0";


int success;
char infoLog[512];
bool colorState;

int main(int argc, const char * argv[]) {
    
    glfwInit();
    
    //stuffs about version of openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework02", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; glewInit();
    
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertextShaderSource, NULL);
    glCompileShader(vertexShader);
    // error check
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    int greenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(greenFragmentShader, 1 , &greenShaderSource, NULL);
    glCompileShader(greenFragmentShader);
    // error check
    glGetShaderiv(greenFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(greenFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
     
    int greenShaderProgram = glCreateProgram();
    glAttachShader(greenShaderProgram, vertexShader);
    glAttachShader(greenShaderProgram, greenFragmentShader);
    glLinkProgram(greenShaderProgram);
    
    // error check
    glGetProgramiv(greenShaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(greenShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    int redFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(redFragmentShader, 1, &redShaderSource, NULL);
    glCompileShader(redFragmentShader);
    //errorCehck
    glGetShaderiv(redFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(redFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    int redShaderProgram = glCreateProgram();
    glAttachShader(redShaderProgram, vertexShader);
    glAttachShader(redShaderProgram, redFragmentShader);
    glLinkProgram(redShaderProgram);

    // error check
    glGetProgramiv(redShaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(redShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(greenFragmentShader);
    glDeleteShader(redFragmentShader);
    
    unsigned int greenVAO;
    unsigned int greenVBO;
    glGenVertexArrays(1, &greenVAO);
    glGenBuffers(1, &greenVBO);
    glBindVertexArray(greenVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, greenVBO);
    float vertices1[] = {
        0.f, -0.5f, 0.f,
        -0.5f, -0.25f, 0.f,
        -0.5f, 0.25f, 0.f,
        0.f, 0.5f, 0.f,
        0.5f, 0.25f, 0.f,
        0.5f, -0.25f, 0.f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    unsigned int redVAO;
    unsigned int redVBO;
    glGenVertexArrays(1, &redVAO);
    glGenBuffers(1, &redVBO);
    glBindVertexArray(redVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, redVBO);
    float vertices2[] = {
        0.f, 0.5f, 0.f,
        -0.5f, 0.25f, 0.f,
        0.5f, 0.25f, 0.f,
        -0.5f, -0.25f, 0.f,
        0.5f, -0.25f, 0.f,
        0.f, -0.5f, 0.f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//    void glVertexAttribPointer(
//         GLuint index,
//         GLint size,
//         GLenum type,
//         GLboolean normalized,
//         GLsizei stride,
//         const void * pointer);
// ref: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //ref: https://www.glfw.org/docs/3.3/group__input.html#ga1caf18159767e761185e49a3be019f8d
    glfwSetKeyCallback(window, press_p_callback);
    
    colorState = true;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window);
        
        if(colorState)
        {
            glUseProgram(greenShaderProgram);
            glBindVertexArray(greenVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        } else
        {
            glUseProgram(redShaderProgram);
            glBindVertexArray(redVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}


void press_p_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // P == int 80
    // ref: https://www.glfw.org/docs/3.0/group__keys.html
    if(key == 80 && action == GLFW_PRESS)
    {
        colorState = !colorState;
    }
}

void processInput(GLFWwindow *window)
{
    //close when excape key pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
         glfwSetWindowShouldClose(window, true);
}
