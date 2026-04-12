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
    static constexpr std::size_t attackFramesPerDirection = 1;
    static constexpr std::size_t directionCount = 4;
    static constexpr std::size_t totalIdleFrames = idleFramesPerDirection * directionCount;
    static constexpr std::size_t totalWalkFrames = walkFramesPerDirection * directionCount;
    static constexpr std::size_t totalAttackFrames = attackFramesPerDirection * directionCount;
    static constexpr std::size_t totalFrames = totalIdleFrames + totalWalkFrames + totalAttackFrames;

    static CharacterConfig defaultConfig();
    static std::array<std::shared_ptr<TDT4102::Image>, totalFrames> createStateDirectionSprites(); //kan flyttes til base?
    InputState readInput(float dt, const TDT4102::AnimationWindow& window) override;
    void drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const override;
    AnimationKey determineAnimationKey(const InputState& inputState) override;
    void applyAnimationFrame(int frame) override;

    std::array<std::shared_ptr<TDT4102::Image>, totalFrames> sprites;
    AnimationKey lastAnimationKey{AnimationState::idle, Direction::down}; //kan settes i base
};