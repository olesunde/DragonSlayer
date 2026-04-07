#pragma once

#include <memory>
#include <Enums.h>
#include <Structs.h>
#include <Animation/Animator.h>

namespace TDT4102 {
    class Image;
    class AnimationWindow;
}

class Camera;
class World;
class Animator;

class Character {
public:
    explicit Character(CharacterConfig&& config);
    virtual ~Character() = default;

    void update(float dt, const TDT4102::AnimationWindow& window, const World& world);
    void updatePosition(float dt, const InputState& input, const World& world);
    void draw(TDT4102::AnimationWindow& window, const Camera& camera) const;
    bool isBlocked(const World& world, float x, float y, float width, float height) const;

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    AnimationState getAnimationState() const { return activeAnimationKey.state; }
    Direction getDirection() const { return activeAnimationKey.direction; }
    int getFrame() const { return animator.getFrame(); }

protected:
    void updateAnimation(float dt, const AnimationKey& key);
    virtual AnimationKey determineAnimationKey(const InputState& inputState) = 0;
    virtual void applyAnimationFrame(int frame) = 0;
    virtual InputState readInput(const TDT4102::AnimationWindow& window) const = 0;
    void setSprite(std::shared_ptr<TDT4102::Image> newSprite);

    Animator animator;
    AnimationKey activeAnimationKey{AnimationState::idle, Direction::down};

private:
    float x;
    float y;
    float width;
    float height;
    float speed;
    float health;
    std::shared_ptr<TDT4102::Image> sprite;
};