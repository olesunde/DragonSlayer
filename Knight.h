#pragma once
#include <Character.h>
#include <array>
#include <memory>

class Camera;

namespace TDT4102 {
    class AnimationWindow;
}

class Knight : public Character {
public:
    Knight();

private:
    static constexpr std::size_t idleFramesPerDirection = 1;
    static constexpr std::size_t walkFramesPerDirection = 4;
    static constexpr std::size_t directionCount = 4;
    static constexpr std::size_t totalIdleFrames = idleFramesPerDirection * directionCount;
    static constexpr std::size_t totalWalkFrames = walkFramesPerDirection * directionCount;
    static constexpr std::size_t totalFrames = totalIdleFrames + totalWalkFrames;

    static CharacterConfig defaultConfig();
    static std::array<std::shared_ptr<TDT4102::Image>, totalFrames> createStateDirectionSprites();
    InputState readInput(const TDT4102::AnimationWindow& window) const override;
    void updateAnimation(float dt) override;

    std::array<std::shared_ptr<TDT4102::Image>, totalFrames> sprites;
    float animationTimer = 0.0f;
    std::size_t currentWalkFrame = 0;
};