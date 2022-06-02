//
//  move_data.h
//  final_project
//
//  Created by 하지훈 on 2022/05/27.
//
#include "movement.h"
#ifndef move_data_h
#define move_data_h

MovementKey heli_1_movementKeys[2] = {
    // time & x,y,z & xAngle, yAngle, zAngle
    MovementKey((float[7]) {1.f, 0.f, 500.f, 2000.f, 0.f, 180.f, 0.f}),
    MovementKey((float[7]) {15.f, 0.f, 550.f, 800.f, 0.f, 180.f, 0.f}),
};

MovementKey heli_2_movementKeys[2] = {
    // time & x,y,z & xAngle, yAngle, zAngle
    MovementKey((float[7]) {1.f, -200.f, 400.f, 2000.f, 0.f, 180.f, 0.f}),
    MovementKey((float[7]) {15.f, -800.f, 400.f, 0.f, 0.f, 180.f, 0.f}),
};

MovementKey heli_3_movementKeys[2] = {
    // time & x,y,z & xAngle, yAngle, zAngle
    MovementKey((float[7]) {1.f, 200.f, 500.f, 2000.f, 0.f, 180.f, 0.f}),
    MovementKey((float[7]) {15.f, 800.f, 350.f, 0.f, 0.f, 180.f, 0.f}),
};


#endif /* move_data_h */
