//
//  movementKey.h
//  final_project
//
//  Created by 하지훈 on 2022/05/26.
//
#pragma once
class MovementKey {
public:
    float time;
    float x, y, z;
    float xAngle, yAngle, zAngle;
    
    MovementKey(float movementData[7])
    {
        time = movementData[0];
        
        x = movementData[1];
        y = movementData[2];
        z = movementData[3];
        
        xAngle = movementData[4];
        yAngle = movementData[5];
        zAngle = movementData[6];
    }
};
