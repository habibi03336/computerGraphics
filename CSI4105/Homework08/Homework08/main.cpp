//
//  main.cpp
//  Homework08
//
//  Created by 하지훈 on 2022/05/03.
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
#include <cube.h>
#include <arcball.h>

using namespace std;

GLFWwindow *glfwAllInit();
GLFWwindow *window = NULL;
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double x, double y);
void render();


int SCR_WIDTH = 600;
int SCR_HEIGHT = 600;
char title[] = "Homework08";

Quadpyra *quadpyra;
Cube *light;

Shader *quadShader = NULL;
Shader *lightShader = NULL;

glm::mat4 projection, view, model;

// for arcball
float arcballSpeed = 0.2f;
static Arcball camArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
static Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;

// for shading
glm::vec3 lightSize(0.1f, 0.1f, 0.1f);
glm::vec3 lightPos(1.f, 1.2f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
glm::vec3 cameraPos(0.0f, 0.0f, 7.0f);
float ambientStrenth = 0.1f;
float specularStrength = 0.5f;
float specularPower = 64.f;
bool FLAT_MODE = true;
bool SMOOTH_MODE = false;
bool shadeMode = FLAT_MODE;

int main(int argc, const char * argv[]) {
    
    // window 관련 설정 추상화
    window = glfwAllInit();
    
    // 필요한 설정
    glEnable(GL_DEPTH_TEST);
    glewExperimental = GL_TRUE; glewInit();
    
    // 기본 배경색 설정
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    // quadpyra용 shader 프로그램, 라이트용 shader 프로그램
    quadShader = new Shader("quadpyra.vs", "quadpyra.fs");
    lightShader = new Shader("light.vs", "light.fs");
    
    // quadpyra 객체, light 객체 생성
    quadpyra = new Quadpyra();
    light = new Cube();
    
    // projection matrix(view -> 2d 평면 해주는 매트릭스)
    projection = glm::perspective(glm::radians(45.0f),
                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    //uniform variable GPU에 전달
    quadShader->use();
    quadShader->setMat4("projection", projection);
    quadShader->setVec3("objectColor", objectColor);
    quadShader->setVec3("lightColor", lightColor);
    quadShader->setVec3("lightPos", lightPos);
    quadShader->setVec3("viewPos", cameraPos);
    quadShader->setFloat("ambientStrenth", ambientStrenth);
    quadShader->setFloat("specularStrength", specularStrength);
    quadShader->setFloat("specularPower", specularPower);
    lightShader->use();
    lightShader->setMat4("projection", projection);
    
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // rendering 과정 추상화
        render();
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void render() {
    // color & detpth buffer에 있던 데이터 전부 지우기
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // 카메라 아크볼 객체가 저장하고 있는 rotation 정보를 통해 적절한 view matrix(world -> model) 만들기
    view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = view * camArcBall.createRotationMatrix();
    // rotation에 따라 변경된 카메라 위치를 생성
    glm::vec4 rotationView = glm::vec4(cameraPos, 1.f) * camArcBall.createRotationMatrix();

    //이 shader 프로그램 관련해서 정보를 전달할 거라고 선언!(이거 까먹어서 고생좀 했다... 휴)
    quadShader->use();
    
    // 쉐이더 프로그램에 unifrom varible로 전달 및 quadpyra 그리기
    quadShader->setVec3("viewPos", glm::vec3(rotationView.x, rotationView.y, rotationView.z));
    quadShader->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = model * modelArcBall.createRotationMatrix();
    quadShader->setMat4("model", model);
    quadpyra->draw(quadShader);
    
    // light shader 프로그램으로 light 그리기
    lightShader->use();
    lightShader->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, lightSize);
    lightShader->setMat4("model", model);
    light->draw(lightShader);
    
    // buffer swap(지연시간을 줄이기 위해 buffer 두 개를 swaping 해가면서 화면에 데이터 전달)
    glfwSwapBuffers(window);
}

//window 관련 작업 추상화 함수
GLFWwindow *glfwAllInit()
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
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    return window;
}

// key board event handler
// R -> 프로그램 초기화, S -> shader 모드 변경(FLAT or SMOOTH), A -> rotation 모드 변경(CAMERA or MODEL)
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        shadeMode = !shadeMode;
        quadpyra->swapShadeMode(shadeMode);
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        arcballCamRot = !arcballCamRot;
        if (arcballCamRot) {
            cout << "ARCBALL: Camera rotation mode" << endl;
        }
        else {
            cout << "ARCBALL: Model  rotation mode" << endl;
        }
    }
}

// mouse event handler
// button 이벤트에 맞는 camArcBall, modelArcBall rotation 시작, 끝을 알림
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (arcballCamRot)
        camArcBall.mouseButtonCallback( window, button, action, mods );
    else
        modelArcBall.mouseButtonCallback( window, button, action, mods );
}

// cusor event handler
// cusor 이벤트에 맞는 camArcBall, modelArcBall rotation 정보 업데이트
void cursor_position_callback(GLFWwindow *window, double x, double y) {
    if (arcballCamRot)
        camArcBall.cursorCallback( window, x, y );
    else
        modelArcBall.cursorCallback( window, x, y );
}

