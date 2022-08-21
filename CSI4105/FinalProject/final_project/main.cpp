//
//  main.cpp
//  final_project
//
//  Created by 하지훈 on 2022/05/22.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <Model.h>
#include <keyframe.h>
#include <text.h>
#include "heli.h"
#include "movement.h"
#include "moveData.h"
#include "background.h"


using namespace std;


float SCR_WIDTH = 600;
float SCR_HEIGHT = 600;
char title[] = "Final Project";

GLFWwindow *glfwAllInit();
GLFWwindow *window = NULL;

void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void render();
void loadTexture();
void spotLightUpdate();
void shaderInit();

glm::mat4 projection;
glm::mat4 model(1.0);
Shader *shader;
Shader *textShader = NULL;
Text *text = NULL;


Heli *heli_1;
Heli *heli_2;
Heli *heli_3;
int heliTotal = 3;
Heli *helis[3];
int heliNum = 0;

Model *cap;
glm::mat4 modelCap;
glm::mat4 modelGround;
Background *ground;

float viewRotate;

//for sunlight
glm::vec3 sunlightColor(0.006f, 0.006f, 0.006f);
// for spot light
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
float ambientStrenth = 0.005f;
float specularStrength = 0.5f;
float specularPower = 32.f;
int spotLightOn;

//time check
float beginT = (float) glfwGetTime();

glm::vec3 finalView = glm::vec3(100.f, 1200.f, 500.f);

int main(int argc, const char * argv[]) {
    srand(glfwGetTime());
    // window 관련 설정 추상화
    window = glfwAllInit();
    // 필요한 설정
    glEnable(GL_DEPTH_TEST);
    glewExperimental = GL_TRUE; glewInit();
    
    heli_1 = new Heli(sizeof(heli_1_movementKeys)/sizeof(heli_1_movementKeys[0]), heli_1_movementKeys, true);
    heli_2 = new Heli(sizeof(heli_2_movementKeys)/sizeof(heli_2_movementKeys[0]), heli_2_movementKeys, false);
    heli_3 = new Heli(sizeof(heli_3_movementKeys)/sizeof(heli_3_movementKeys[0]), heli_3_movementKeys, true);
    helis[0] = heli_1; helis[1] = heli_2; helis[2] = heli_3;

    
    shader = new Shader("res/shaders/shader.vs", "res/shaders/shader.fs");
    cap = new Model((GLchar *) "res/models/cap/cap.obj");
    
    char imgPath[] = "ground.jpeg";
    ground = new Background(100.f, imgPath);
    
    projection = glm::perspective(glm::radians(120.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.f);
 
    //
    modelCap = glm::translate(model, glm::vec3(125.f, -280.f, 300.f));
    modelCap = glm::scale(modelCap, glm::vec3(200.f, 200.f, 200.f));
    modelCap = glm::rotate(modelCap, glm::radians(-20.f) , glm::vec3(1.0f, 0.0f, 0.0f)); //x
    modelCap = glm::rotate(modelCap, glm::radians(-30.f) , glm::vec3(0.0f, 1.0f, 0.0f)); //y
    modelCap = glm::rotate(modelCap, glm::radians(40.f) , glm::vec3(0.0f, 0.0f, 1.0f)); //z
    
    modelGround = glm::scale(model, glm::vec3(100000.f, 100000.f, 100000.f));
    
    
    textShader = new Shader("res/shaders/text.vs", "res/shaders/text.frag");
    textShader->use();
    textShader->setMat4("projection", projection);
    text = new Text((char*)"res/fonts/arial.ttf", textShader, SCR_WIDTH, SCR_HEIGHT);

    shader->use();
    shader->setMat4("projection", projection);
    shaderInit();
    
    // 기본 배경색 설정
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // rendering 과정 추상화
        render();
        
        glfwSwapBuffers( window );
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}

void render()
{
    // color & detpth buffer에 있던 데이터 전부 지우기
    
    shader->use();
    spotLightUpdate();
    float currentTime = glfwGetTime();
    
    //prevent filckering
    if (currentTime > 55.f) glClearColor(1.f, 1.f, 1.f, 1.f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //spotlight control by time
    spotLightOn = currentTime > 9 && currentTime < 55 ? 1 : 0;
    shader->setInt("spotLightOn", spotLightOn);
    
    // sunlight brightness control by time
    if (currentTime > 40 && currentTime < 55)
    {
        sunlightColor = sunlightColor*(1.f + 0.005f);
    }
    
    //view control by time
    if(currentTime < 30 )
    {
        glm::vec3 heli_position = helis[heliNum]->getCoords();
        glm::mat4 view = glm::lookAt(heli_position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        shader->setMat4("view", view);
        glm::vec4 rotationView = glm::vec4(heli_position, 1.f);
        shader->setVec3("viewPos", glm::vec3(rotationView.x, rotationView.y, rotationView.z));
        viewRotate = currentTime;
    } else if ( currentTime < 55 )
    {
        float rotate = (viewRotate - glfwGetTime())*20;
        if (rotate < -360.f) rotate = -360.f;
        glm::mat4 view = glm::lookAt(finalView, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        view = glm::rotate(view, glm::radians(rotate), glm::vec3(0.f, 1.f, 0.f));

        shader->setMat4("view", view);
        glm::vec4 rotationView = glm::vec4(finalView, 1.f);
        shader->setVec3("viewPos", glm::vec3(rotationView.x, rotationView.y, rotationView.z));
    } else
    {
        modelCap = glm::rotate(modelCap, glm::radians(0.5f), glm::vec3(0.f, 1.f, 0.f));
    }

    
    if (sunlightColor.x > 0.8f) shader->setVec3("sunlight.color", glm::vec3(0.8f));
    else shader->setVec3("sunlight.color", sunlightColor);
    
    shader->setMat4("model", modelCap);
    cap->Draw(shader);
    
    //text rendering
    if (currentTime > 55.f) {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
           
        text->RenderText("Refounded", 75.f, 500.f, 2.f, glm::vec3(0.f));
        text->RenderText("Classic Newera", 50.0f, 25.0f, 1.5f, glm::vec3(0.f));
        return;
    }
    
    
    //spotlight not effect other than cap
    shader->setInt("spotLightOn", 0);
    
    shader->use();
    shader->setVec3("sunlight.color", sunlightColor*20.f);
    shader->setMat4("model", modelGround);
    ground->Draw(shader);
    
    float elapsedT = (float)currentTime - beginT;
    for(int i = 0; i < heliTotal; i++)
    {
        helis[i]->draw(shader, model, elapsedT);
    }
    
}

void shaderInit()
{
    shader->use();
    glm::vec3 diffuseColor = lightColor * glm::vec3(3.f);
    glm::vec3 ambientColor(ambientStrenth);
    shader->setVec3("sunlight.color", sunlightColor);
    shader->setVec3("sunlight.pos", glm::vec3(1000.f));
    shader->setVec3("spotLightCommon.ambient", ambientColor);
    shader->setVec3("spotLightCommon.diffuse", diffuseColor);
    shader->setFloat("spotLightCommon.cutOff", glm::cos(glm::radians(12.f)));
    shader->setFloat("spotLightCommon.innerCutOff", glm::cos(glm::radians(6.f)));
    shader->setVec3("spotLightCommon.specular", 1.f, 1.f, 1.f);
    shader->setFloat("spotLightCommon.attenConstant", 1.0f);
    shader->setFloat("spotLightCommon.attenLinearTerm", .000001f);
    shader->setFloat("spotLightCommon.attenQuadTerm", .000001f);
    
    shader->setFloat("material.specular", specularStrength);
    shader->setFloat("material.shininess", specularPower);
}

void spotLightUpdate()
{
    glm::vec3 heliPositions[3];
    for(int i = 0; i < heliTotal; i++)
    {
        glm::vec3 heliPositions_ = helis[i]->getCoords();
        heliPositions_.x += (float) (rand() % 40);
        heliPositions_.y += (float) (rand() % 40);
        heliPositions_.z += (float) (rand() % 40);
        heliPositions[i] = heliPositions_;
    }
    
    shader->use();
    shader->setVec3("spotLight.position", heliPositions[0]);
    shader->setVec3("spotLight.direction", -heliPositions[0]);

    shader->setVec3("spotLight2.position", heliPositions[1]);
    shader->setVec3("spotLight2.direction", -heliPositions[1]);

    shader->setVec3("spotLight3.position", heliPositions[2]);
    shader->setVec3("spotLight3.direction", -heliPositions[2]);
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
    return window;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        heliNum = (heliNum + 1) % 3;
    }
}
