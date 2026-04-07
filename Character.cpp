
#include <Character.h>
#include <Camera.h>
#include <World.h>
#include "AnimationWindow.h"
#include <cmath>
#include <utility>

Character::Character(CharacterConfig&& config)
    : x(config.spawnX),
      y(config.spawnY),
      width(config.width),
      height(config.height),
      speed(config.speed),
      direction(Direction::down),
      animationState(AnimationState::idle),
      sprite(std::move(config.sprite)) {}

void Character::update(float dt, const TDT4102::AnimationWindow& window, const World& world) {
    const InputState input = readInput(window);
    handleInput(input);
    updatePosition(dt, input, world);
    updateAnimation(dt);
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

void Character::handleInput(const InputState& inputState) {
    animationState = AnimationState::idle;

    if (inputState.left) {
        direction = Direction::left;
        animationState = AnimationState::walking;
    }

    if (inputState.right) {
        direction = Direction::right;
        animationState = AnimationState::walking;
    }

    if (inputState.up) {
        direction = Direction::up;
        animationState = AnimationState::walking;
    }

    if (inputState.down) {
        direction = Direction::down;
        animationState = AnimationState::walking;
    }
}

void Character::setSprite(std::shared_ptr<TDT4102::Image> newSprite) {
    sprite = std::move(newSprite);
}

void Character::draw(TDT4102::AnimationWindow& window, const Camera& camera) const {
    TDT4102::Point topLeft = camera.worldToCamera(x, y);
    window.draw_image(topLeft, *sprite, static_cast<int>(width), static_cast<int>(height));
}

bool Character::isBlocked(const World& world, float x, float y, float width, float height) const {
    float left = x;
    float right = x + width - 1;
    float top = y;
    float bottom = y + height - 1;

    int leftCol = static_cast<int>(static_cast<int>(left) / World::tileSize);
    int rightCol = static_cast<int>(static_cast<int>(right) / World::tileSize);
    int topRow = static_cast<int>(static_cast<int>(top) / World::tileSize);
    int bottomRow = static_cast<int>(static_cast<int>(bottom) / World::tileSize);
        
    return world.isBlockedTile(topRow, leftCol) || 
        world.isBlockedTile(topRow, rightCol) || 
        world.isBlockedTile(bottomRow, leftCol) || 
        world.isBlockedTile(bottomRow, rightCol);
}
