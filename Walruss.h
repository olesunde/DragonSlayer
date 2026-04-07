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
    void setTarget(const Character& knight);

private:
    static CharacterConfig defaultConfig();
    InputState readInput(const TDT4102::AnimationWindow& window) const override;
    AnimationKey determineAnimationKey(const InputState& inputState) override;
    void applyAnimationFrame(int frame) override;

    static constexpr std::size_t totalFrames = 1;
    std::array<std::shared_ptr<TDT4102::Image>, totalFrames> sprites;
    AnimationKey lastDirectionKey{AnimationState::idle, Direction::left};

    const Character* target = nullptr;
};