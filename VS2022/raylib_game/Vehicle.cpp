#include "raylib.h"
#include "raymath.h"
#include <iostream>
//#include "C:\proj\vs\raylib-game-template-main\projects\VS2022\raylib_game\Header.h"
#include "Header.h"

Vehicle::Vehicle() {
    acceleration = Vector2({ 0, 0 });
    velocity = Vector2({ 0, 0 });
    location = Vector2({ 0, 0 });
    maxspeed = 2;
    maxforce = 0.1f;
    angle = getAngle();
    invsTarget = Vector2({ 0, 0 });
    foodCnt = 0;
    eatingRange = 0;
    detectionRange = 0;
    lastEaten = -3;
    rechargeTime = 3;
}

Vehicle::Vehicle(float x, float y, int range) {//old default
    acceleration = Vector2({ 0, 0 });
    velocity = Vector2({ 0, 0 });
    location = Vector2({ x, y });
    maxspeed = 2;
    maxforce = 0.1f;
    angle = getAngle();
    invsTarget = Vector2({ 0, 0 });
    foodCnt = 0;
    eatingRange = range;
    detectionRange = 0;
    lastEaten = -3;
    rechargeTime = 3;
}

Vehicle::Vehicle(float x, float y, float speed, float force, int range, int detect, float recharge) {
    acceleration = Vector2({ 0, 0 });
    velocity = Vector2({ 0, 0 });
    location = Vector2({ x, y });
    maxspeed = speed;
    maxforce = force;
    angle = getAngle();
    invsTarget = Vector2({ 0, 0 });
    foodCnt = 0;
    eatingRange = range;
    detectionRange = detect;
    lastEaten = -3;
    rechargeTime = recharge;
}

void Vehicle::update() {
    velocity = Vector2Add(velocity, acceleration);
    velocity = Vector2ClampValue(velocity, 0.0f, maxspeed);
    location = Vector2Add(location, velocity);
    acceleration = Vector2Scale(acceleration, 0);
    //std::cout << "\n" << location.x << ", " << location.y << "\n";
}

void Vehicle::applyForce(Vector2 force) {
    acceleration = Vector2Add(acceleration, force);
}

float Vehicle::getDistanceTo(Vector2 point) {
    float dx = location.x - point.x;
    float dy = location.y - point.y;
    return std::sqrt(dx * dx + dy * dy);
}

void Vehicle::seek(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, location);
    desired = Vector2Normalize(desired);
    desired = Vector2Multiply(desired, Vector2({ maxspeed, maxspeed }));
    Vector2 steer = Vector2Subtract(desired, velocity);
    steer = Vector2ClampValue(steer, -maxforce, maxforce);
    applyForce(steer);
}

float Vehicle::getAngle() {
    return atan2(velocity.y, velocity.x) * (180 / PI) + 90;
}

void Vehicle::draw(Texture texture) {
    DrawTexturePro(texture, Rectangle({ 0, 0, (float)texture.width, (float)texture.height }), 
        Rectangle({ location.x, location.y, (float)texture.width, (float)texture.height }),
        Vector2({ texture.width / 2.0f, texture.height / 2.0f }), angle = Lerp(angle, getAngle(), 0.1f), WHITE);
}

void Vehicle::reset(Vector2 pos) {
    acceleration = Vector2({ 0, 0 });
    velocity = Vector2({ 0, 0 });
    invsTarget = Vector2({ pos.x, pos.y });;
    foodCnt = 0;
    lastEaten = -3;
}

void Vehicle::resetAndIncreaseStats(Vector2 pos, float speed, float force, int range, int detect, float recharge) {
    reset(pos);
    maxspeed += speed;
    maxforce += force;
    if (eatingRange != 5)
        eatingRange += range;
    if(detectionRange != 0)
        detectionRange += detect;
    rechargeTime -= recharge;
}