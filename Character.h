#pragma once
#include "AnimationWindow.h"
#include <memory.h>

class World;
class Camera;

class Character {
private:
    float x;
    float y;
    float width;
    float height;
    std::unique_ptr<TDT4102::Image> sprite;

    TDT4102::AnimationWindow& window;
    World& world;

public:
    Character(TDT4102::AnimationWindow& window, World& world);

    enum class Type {
        knight,
        princess,
        dragon
    };

    struct Config {
        TDT4102::Point spawnPoint;
        std::unique_ptr<TDT4102::Image> sprite;
        float width;
        float height;
    };

    Config config(Type type);

    void handleInput();
    void updatePosition();
    void draw(const Camera& camera);
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    /*
    bool isBlocked(float x, float y) const;
    */  
};