
#include <Character.h>
#include <Camera.h>
#include <World.h>
#include "AnimationWindow.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <Timer.h>

Character::Character(CharacterConfig&& config)
    : x(config.spawnX),
      y(config.spawnY),
      width(config.width),
      height(config.height),
      speed(config.speed),
      health(config.health),
      maxHealth(config.maxHealth),
      attackRange(config.attackRange),
      attackDamage(config.attackDamage),
      attackCooldown(config.attackCooldown),
      damageCooldown(config.damageCooldown),
      attackWidth(config.attackWidth),
      attackHeight(config.attackHeight),
      attackEffect(std::move(config.attackEffect)),
      sprite(std::move(config.sprite)),
      soundEffect(std::move(config.soundEffect)) {}

void Character::update(float dt, TDT4102::AnimationWindow& window, const World& world) {
    if (dead) {
        return;
    }

    const InputState input = readInput(dt, window);

    for (auto& [target, timer] : targetDamageTimers) {
        (void)target;
        timer += dt;
    }

    if (isAttacking) {
        attackEffectTimer += dt;
        if (attackEffectTimer >= attackEffectDuration) {
            isAttacking = false;
        }
    }

    attackTimer += dt;
    if (input.attack && attackTimer >= attackCooldown) {
        attackTimer = 0.0f;
        isAttacking = true;
        window.play_audio(*soundEffect, 1);
        attackEffectTimer = 0.0f;
        attackEnemy(window, input);
    }

    updatePosition(dt, input, world);
    activeAnimationKey = determineAnimationKey(input);
    updateAnimation(dt, activeAnimationKey);
}

void Character::addTarget(const std::shared_ptr<Character>& target) {
    if (!target || target.get() == this) {
        return;
    }

    if (targetDamageTimers.contains(target)) {
        return;
    }

    targetDamageTimers[target] = 0.0f;
}

void Character::clearTargets() {
    targetDamageTimers.clear();
}

void Character::updatePosition(float dt, const InputState& input, const World& world) {
    float dx = 0.0f;
    float dy = 0.0f;

    if (input.left) { dx-= 1.0f; }
    if (input.right) { dx += 1.0f; }
    if (input.up) { dy -= 1.0f; }
    if (input.down) { dy += 1.0f; }

    const float lengthSquared = dx * dx + dy * dy;
    if (lengthSquared > 0.0f) {
        const float invLength = 1.0f / std::sqrt(lengthSquared);
        dx *= invLength;
        dy *= invLength;
    }

    const float stepX = dx * speed * dt;
    const float stepY = dy * speed * dt;

    const float candidateX = x + stepX;
    if (!isBlocked(world, candidateX, y, width, height)) {
        x = candidateX;
    }

    const float candidateY = y + stepY;
    if (!isBlocked(world, x, candidateY, width, height)) {
        y = candidateY;
    }
}

void Character::updateAnimation(float dt, const AnimationKey& key) {
    animator.play(key);
    animator.update(dt);
    applyAnimationFrame(animator.getFrame());
}

void Character::setSprite(std::shared_ptr<TDT4102::Image> newSprite) {
    sprite = std::move(newSprite);
}

float Character::getHealthRatio() const {
    return std::clamp(health / maxHealth, 0.0f, 1.0f);
}

void Character::takeDamage(float amount) {
    if (amount <= 0.0f || dead) {
        return;
    }

    health = std::max(0.0f, health - amount);
    if (health <= 0.0f) {
        dead = true;
        isAttacking = false;
    }
}

void Character::draw(TDT4102::AnimationWindow& window, const Camera& camera) const {
    TDT4102::Point characterTopLeft = camera.worldToCamera(x, y);
    window.draw_image(characterTopLeft, *sprite, static_cast<int>(width), static_cast<int>(height));

    if (isAttacking && attackEffect) {
        TDT4102::Point attackTopLeft = camera.worldToCamera(static_cast<float>(attackEffectTopLeft.x), static_cast<float>(attackEffectTopLeft.y));
        window.draw_image(
            attackTopLeft, *attackEffect,
            static_cast<int>(attackRenderWidth), static_cast<int>(attackRenderHeight),
            attackEffectAngle, attackEffectOrigin
        );
    }

    drawHealthBar(window, camera);
}

bool Character::isBlocked(const World& world, float x, float y, float width, float height) const {
    int leftCol = static_cast<int>(static_cast<int>(x) / World::tileSize);
    int rightCol = static_cast<int>(static_cast<int>(x + width - 1) / World::tileSize);
    int topRow = static_cast<int>(static_cast<int>(y) / World::tileSize);
    int bottomRow = static_cast<int>(static_cast<int>(y + height - 1) / World::tileSize);
        
    const bool blockedByWorld = world.isBlockedTile(topRow, leftCol) ||
        world.isBlockedTile(topRow, rightCol) ||
        world.isBlockedTile(bottomRow, leftCol) ||
        world.isBlockedTile(bottomRow, rightCol);

    return blockedByWorld;
}

void Character::attackEnemy(TDT4102::AnimationWindow& window, const InputState& input) {
    (void)window;
    const float margin = 0.0f;
    float effectWidth = attackWidth;
    float effectHeight = attackHeight;

    Direction attackDirection = getDirection();
    if (input.left) { attackDirection = Direction::left; }
    if (input.right) { attackDirection = Direction::right; }
    if (input.up) { attackDirection = Direction::up; }
    if (input.down) { attackDirection = Direction::down; }

    switch (attackDirection) {
    case Direction::left:
        effectWidth *= 0.75f;
        attackEffectTopLeft = {static_cast<int>(getX() - effectWidth - margin), static_cast<int>(getY())};
        attackEffectAngle = 90.0f;
        break;
    case Direction::right:
        effectWidth *= 0.75f;
        attackEffectTopLeft = {static_cast<int>(getX() + getWidth() + margin), static_cast<int>(getY())};
        attackEffectAngle = 270.0f;
        break;
    case Direction::up:
        effectHeight *= 0.75f;
        attackEffectTopLeft = {static_cast<int>(getX()), static_cast<int>(getY() - effectHeight - margin)};
        attackEffectAngle = 180.0f;
        break;
    case Direction::down:
        effectHeight *= 0.75f;
        attackEffectTopLeft = {static_cast<int>(getX()), static_cast<int>(getY() + getHeight() + margin)};
        attackEffectAngle = 0.0f;
        break;
    }

    attackRenderWidth = effectWidth;
    attackRenderHeight = effectHeight;

    attackEffectOrigin = {static_cast<int>(attackRenderWidth * 0.5f), static_cast<int>(attackRenderHeight * 0.5f)};

    const float centerOfAttackX = static_cast<float>(attackEffectTopLeft.x) + attackRenderWidth * 0.5f;
    const float centerOfAttackY = static_cast<float>(attackEffectTopLeft.y) + attackRenderHeight * 0.5f;

    for (auto& [target, takeDamageTimer] : targetDamageTimers) {
        if (!target || target.get() == this || target->isDead()) {
            continue;
        }

        if (takeDamageTimer < damageCooldown) {
            continue;
        }

        const double dx = static_cast<double>(centerOfAttackX - target->getCenterX());
        const double dy = static_cast<double>(centerOfAttackY - target->getCenterY());
        const double distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= static_cast<double>(attackRange)) {
            target->takeDamage(attackDamage);
            takeDamageTimer = 0.0f;
        }
    }
}

