#pragma once
#include <memory>

namespace TDT4102 {
    class Image;
    class AnimationWindow;
}
class Camera;
class World;

struct InputState {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
};

enum class Direction {
    left,
    right,
    up,
    down
};

enum class AnimationState {
    idle,
    walking,
};

struct CharacterConfig {
    float spawnX = 0.f;
    float spawnY = 0.f;
    float width = 0.f;
    float height = 0.f;
    float speed = 0.f;
    std::shared_ptr<TDT4102::Image> sprite;
};

class Character {
public:
    explicit Character(CharacterConfig&& config);
    virtual ~Character() = default;

    void update(float dt, const TDT4102::AnimationWindow& window, const World& world);
    void updatePosition(float dt, const InputState& input, const World& world);
    void draw(TDT4102::AnimationWindow& window, const Camera& camera) const;
    void handleInput(const InputState& inputState);
    bool isBlocked(const World& world, float x, float y, float width, float height) const;

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

protected:
    virtual void updateAnimation(float dt) = 0;
    virtual InputState readInput(const TDT4102::AnimationWindow& window) const = 0;
    void setSprite(std::shared_ptr<TDT4102::Image> newSprite);
    
    Direction direction;
    AnimationState animationState;

private:

    float x;
    float y;
    float width;
    float height;
    float speed;
    std::shared_ptr<TDT4102::Image> sprite;
};