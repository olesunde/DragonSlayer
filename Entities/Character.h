#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
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

    void update(float dt, TDT4102::AnimationWindow& window, const World& world);
    void updatePosition(float dt, const InputState& input, const World& world);
    virtual void draw(TDT4102::AnimationWindow& window, const Camera& camera) const;
    bool isBlocked(const World& world, float x, float y, float width, float height) const;
    void addTarget(const std::shared_ptr<Character>& target);
    void clearTargets();
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getCenterX() const { return x + width * 0.5f; }
    float getCenterY() const { return y + height * 0.5f; }
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    bool isDead() const { return dead; }
    float getHealthRatio() const;
    float getAttackRange() const { return attackRange; }
    float getAttackDamage() const { return attackDamage; }

    void takeDamage(float amount);
    AnimationState getAnimationState() const { return activeAnimationKey.state; }
    Direction getDirection() const { return activeAnimationKey.direction; }
    int getFrame() const { return animator.getFrame(); }
    virtual void drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const = 0;

protected:
    virtual void updateAnimation(float dt, const AnimationKey& key);
    void attackEnemy(TDT4102::AnimationWindow& window, const InputState& input);
    virtual AnimationKey determineAnimationKey(const InputState& inputState) = 0;
    virtual void applyAnimationFrame(int frame) = 0;
    virtual InputState readInput(float dt, const TDT4102::AnimationWindow& window) = 0;
    void setSprite(std::shared_ptr<TDT4102::Image> newSprite);
    Animator animator;
    AnimationKey activeAnimationKey{AnimationState::idle, Direction::down};

private:
    float x;
    float y;
    float width;
    float height;
    float speed;
    bool dead = false;

    float health;
    float maxHealth;

    float attackRange;
    float attackDamage;
    float attackCooldown;
    float attackWidth;
    float attackHeight;
    float attackRenderWidth = 0.0f;
    float attackRenderHeight = 0.0f;

    float attackTimer = 0.0f;
    float damageCooldown;
    bool isAttacking = false;
    float attackEffectTimer = 0.0f;
    float attackEffectDuration = 0.25f;
    TDT4102::Point attackEffectTopLeft{0, 0};
    double attackEffectAngle = 0.0;
    TDT4102::Point attackEffectOrigin{0, 0};
    std::shared_ptr<TDT4102::Image> attackEffect;
    std::unordered_map<std::shared_ptr<Character>, float> targetDamageTimers;
    std::shared_ptr<TDT4102::Image> sprite;
    std::shared_ptr<TDT4102::Audio> soundEffect;
};