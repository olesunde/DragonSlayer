#pragma once
#include "AnimationWindow.h"

class Character;

class Camera {
private:
    float x = 0.f;
    float y = 0.f;
    TDT4102::AnimationWindow& window;

public:
    Camera(TDT4102::AnimationWindow& window);
    
    void follow(const Character& character);

    TDT4102::Point worldToCamera(float worldX, float worldY) const;

    float getX() const;
    float getY() const;
};
