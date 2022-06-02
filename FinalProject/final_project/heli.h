//
//  heli.h
//  final_project
//
//  Created by 하지훈 on 2022/05/26.
//
#ifndef heli_h
#define heli_h
#include <Model.h>
#include <shader.h>
#include "movement.h"
#include <keyframe.h>
#include <cmath>

class Heli {
private:
    glm::vec3 propellerOffset = glm::vec3(0.0f, 29.1984f, 27.912f);
    glm::vec3 tailPropellerOffset = glm::vec3(-1.50733f, 34.2354f, -67.9586f);
    float propellerCoeff = 60.f;
    KeyFraming *xTKF, *yTKF, *zTKF;
    KeyFraming *xRKF, *yRKF, *zRKF;
    float xTrans, yTrans, zTrans;
    float xAngle, yAngle, zAngle;
    float timeT;
    float animEndTime;
    enum RenderMode { INIT, ANIM, STOP, LOOP};
    RenderMode renderMode;
    float patrolRadius;
    bool CCN;
    bool isUpper;
    int tolerance = 0;
    Model *heliBody;
    Model *heliPro;
    Model *heliProTail;
    
public:
    Heli(int n, MovementKey movementKeys[], bool patrolDirection) {
        CCN = patrolDirection;
        this->patrolRadius = sqrt(pow(movementKeys[n-1].x,2) + pow(movementKeys[n-1].z,2));
        isUpper = movementKeys[n-1].z < 0 ? true : false;
        renderMode = ANIM;
    
        heliBody = new Model((GLchar *) "res/models/blackhawk/body.obj");
        heliPro = new Model((GLchar *) "res/models/blackhawk/propeller.obj");
        heliProTail = new Model((GLchar *) "res/models/blackhawk/tail_propeller.obj");
        
        xTKF = new KeyFraming(n);
        yTKF = new KeyFraming(n);
        zTKF = new KeyFraming(n);
        
        xRKF = new KeyFraming(n);
        yRKF = new KeyFraming(n);
        zRKF = new KeyFraming(n);
        
        for(int i = 0; i < n; i++)
        { 
            MovementKey movementKey = movementKeys[i];
            float time = movementKey.time;
           
            xTKF->setKey(i, time, movementKey.x);
            yTKF->setKey(i, time, movementKey.y);
            zTKF->setKey(i, time, movementKey.z);
            
            xRKF->setKey(i, time, movementKey.xAngle);
            yRKF->setKey(i, time, movementKey.yAngle);
            zRKF->setKey(i, time, movementKey.zAngle);
           
            if (i == n-1 )
            {
                animEndTime = time;
            }
        }
    }
    
    
    
    void draw(Shader *shader, glm::mat4 model, float t) {
        
        timeT = t;
        updateAnimData();
        
        shader->use();
        
        model = glm::translate(model, glm::vec3(xTrans, yTrans, zTrans));
        glm::vec3 eulerAngles(glm::radians(xAngle), glm::radians(yAngle), glm::radians(zAngle));
        glm::quat q(eulerAngles);
        glm::mat4 rotMatrix = q.operator glm::mat4x4();
        model = model * rotMatrix;
        
        shader->setMat4("model", model);
        heliBody->Draw(shader);
        
        glm::mat4 modelPro = glm::translate(model, propellerOffset);
        modelPro = glm::rotate(modelPro, (GLfloat)glfwGetTime() * propellerCoeff, glm::vec3(0.0f,1.0f,0.0f));
        shader->setMat4("model", modelPro);
        heliPro->Draw(shader);
        
        glm::mat4 modelProTail = glm::translate(model, tailPropellerOffset);
        modelProTail = glm::rotate(modelProTail, (GLfloat)glfwGetTime() * propellerCoeff, glm::vec3(1.0f,0.0f,0.0f));
        shader->setMat4("model", modelProTail);
        heliProTail->Draw(shader);
    }
    
    void updateAnimData()
    {
        if (renderMode == ANIM)
        {
            if (timeT > animEndTime)
            {
                renderMode = LOOP;
                return;
            }
            xTrans = xTKF->getValLinear(timeT);
            yTrans = yTKF->getValLinear(timeT);
            zTrans = zTKF->getValLinear(timeT);
            
            xAngle = xRKF->getValLinear(timeT);
            yAngle = yRKF->getValLinear(timeT);
            zAngle = zRKF->getValLinear(timeT);
        }
        else if (renderMode == LOOP)
        {
            loopAnim(0.01f);
        }
    }
    
    void loopAnim(float t)
    {
        
        float theta = acos(xTrans / patrolRadius);
        
        if ( (abs(theta) > 3.13f || abs(theta) < 0.01f) && tolerance <= 0)
        {
            isUpper = !isUpper;
            tolerance = 10;
        }
        
        if (isUpper)
        {
            theta = 6.28f - theta;
        }
        
        tolerance -= 1;
        if (!CCN) t *= -1;
        xTrans = patrolRadius * cos(theta + t*0.8f);
        zTrans = patrolRadius * sin(theta + t*0.8f);
    }
    
    glm::vec3 getCoords()
    {
        return glm::vec3(xTrans, yTrans, zTrans);
    }
};

#endif /* heli_h */
