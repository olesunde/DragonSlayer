
#include <Character.h>
#include <World.h>
#include <Camera.h>

Character::Character(TDT4102::AnimationWindow& window, World& world)
    : window(window), 
      world(world) {}

Character::Config Character::config(Character::Type type) {
    switch (type) {
        case Type::knight:
            return {};
        case Type::princess:
            return {};
        case Type::dragon:
            return {};
    }
}

float Character::getX() const {
    return x;
}

float Character::getY() const {
    return y;
}

float Character::getWidth() const {
    return width;
}

float Character::getHeight() const {
    return height;
}

void Character::handleInput() {
    float step = 4.0f;

    if (window.is_key_down(KeyboardKey::LEFT)) {
            x -= step;
    }
    if (window.is_key_down(KeyboardKey::RIGHT)) {
            x += step;
    }
    if (window.is_key_down(KeyboardKey::UP)) {
            y -= step;
    }
    if (window.is_key_down(KeyboardKey::DOWN)) {
            y += step;
    }

    /*
    if (!isBlocked(newPositionX, y)) {
        x = newPositionX;
    }

    if (!isBlocked(x, newPositionY)) {
        y = newPositionY;
    }*/
}

void Character::draw(const Camera& camera) {
    TDT4102::Point topLeft = camera.worldToCamera(x, y);
    window.draw_image(topLeft, *sprite, static_cast<int>(width), static_cast<int>(height));
}

void Character::updatePosition() {
    handleInput();
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
