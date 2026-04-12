#pragma once
#include <Character.h>
#include <array>
#include <memory>

class Camera;

namespace TDT4102 {
	class AnimationWindow;
}

class Dragon : public Character {
public:
	Dragon();
	void setTarget(const Character& knight);

private:
	static CharacterConfig defaultConfig();
	InputState readInput(float dt, const TDT4102::AnimationWindow& window) override;
	void draw(TDT4102::AnimationWindow& window, const Camera& camera) const override;
	void drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const override;
	AnimationKey determineAnimationKey(const InputState& inputState) override;
	void updateAnimation(float dt, const AnimationKey& key) override;
	void applyAnimationFrame(int frame) override;

	float windupTimer = 0.0f;
    float movementTimer = 0.0f;
	static constexpr std::size_t totalFrames = 2;
    static constexpr std::size_t deadFrames = 2;
    static constexpr std::size_t eatingFrames = 2;
    static constexpr std::size_t spawnFrames = 4;
    std::array<std::shared_ptr<TDT4102::Image>, totalFrames> sprites;
    std::array<std::shared_ptr<TDT4102::Image>, deadFrames> deadSprites;
    std::array<std::shared_ptr<TDT4102::Image>, spawnFrames> spawnSprites;
    std::array<std::shared_ptr<TDT4102::Image>, eatingFrames> eatingSprites;
	bool isSpawning = true;
	float spawnTimer = 0.0f;
	float spawnFrameDuration = 0.25f;
	AnimationKey lastAnimationKey{AnimationState::idle, Direction::down};

	const Character* target = nullptr;
};
