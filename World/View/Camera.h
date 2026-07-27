#pragma once
#include "AnimationWindow.h"

class Character;

class Camera {
public:
    void follow(const TDT4102::AnimationWindow& window, const Character& character);

    TDT4102::Point worldToCamera(float worldX, float worldY) const;

    float getX() const { return x; };
    float getY() const { return y; };

private:
    float x = 0.f;
    float y = 0.f;
};
