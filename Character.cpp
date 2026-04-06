
#include <Character.h>
#include <Camera.h>
#include "AnimationWindow.h"
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

void Character::update(float dt, const TDT4102::AnimationWindow& window) {
    const InputState input = readInput(window);
    handleInput(input);
    updatePosition(dt, input);
    updateAnimation(dt);
}

void Character::updatePosition(float dt, const InputState& input) {
    float dx = 0.0f;
    float dy = 0.0f;

    if (input.left) { dx -= speed * dt; }
    if (input.right) { dx += speed * dt; }
    if (input.up) { dy -= speed * dt; }
    if (input.down) { dy += speed * dt; }

    x += dx;
    y += dy;
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

/*
bool Character::isBlocked(float x, float y) const {
    float left = x;
    float right = x + width - 1;
    float top = y;
    float bottom = y + height - 1;

    int leftCol = static_cast<int>(static_cast<int>(left) / World::tileSize);
    int rightCol = static_cast<int>(static_cast<int>(right) / World::tileSize);
    int topRow = static_cast<int>(static_cast<int>(top) / World::tileSize);
    int bottomRow = static_cast<int>(static_cast<int>(bottom) / World::tileSize);

        // Spør world instans om det er en WALL i veien
        return !world.isWalkable(topRow, leftCol) || 
            !world.isWalkable(topRow, rightCol) || 
            !world.isWalkable(bottomRow, leftCol) || 
            !world.isWalkable(bottomRow, rightCol);
}*/
