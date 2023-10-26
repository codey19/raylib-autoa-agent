#pragma once
#include <iostream>
#include "raylib.h"
class Vehicle {
public:
    Vector2 location;
    Vector2 velocity;
    Vector2 acceleration;
    float r;
    float maxforce;
    float maxspeed;
    float angle;
    Vector2 invsTarget;
    int foodCnt;
    int eatingRange;
    int detectionRange;
    float rechargeTime;
    float lastEaten;

    Vehicle::Vehicle();
    Vehicle::Vehicle(float x, float y, int range);
    Vehicle::Vehicle(float x, float y, float speed, float force, int range, int detect, float rechargeTime);
    void Vehicle::update();
    void Vehicle::applyForce(Vector2 force);
    void Vehicle::seek(Vector2 target);
    float Vehicle::getDistanceTo(Vector2 point);
    void Vehicle::draw(Texture texture);
    float Vehicle::getAngle();
    void Vehicle::reset(Vector2 pos);
    void Vehicle::resetAndIncreaseStats(Vector2 pos, float speed, float force, int range, int detect, float recharge);
};