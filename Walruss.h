#pragma once
#include <Character.h>
#include <array>
#include <memory>

class Camera;

namespace TDT4102 {
    class AnimationWindow;
}

class Walruss : public Character {
public:
    Walruss();

private:
    static CharacterConfig defaultConfig();
    InputState readInput(const TDT4102::AnimationWindow& window) const override;
    void updateAnimation(float dt) override;
};