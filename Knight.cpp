#include <Knight.h>
#include "AnimationWindow.h"
#include <World.h>
#include <memory>
#include <array>
#include <string>

Knight::Knight()
	: Character(defaultConfig()),
	  sprites(createStateDirectionSprites()) {
	animator.addAnimation({AnimationState::idle, Direction::left}, Animation(0, 1, 0.12f));
	animator.addAnimation({AnimationState::idle, Direction::right}, Animation(1, 1, 0.12f));
	animator.addAnimation({AnimationState::idle, Direction::up}, Animation(2, 1, 0.12f));
	animator.addAnimation({AnimationState::idle, Direction::down}, Animation(3, 1, 0.12f));

	animator.addAnimation({AnimationState::walking, Direction::left}, Animation(4, 4, 0.12f));
	animator.addAnimation({AnimationState::walking, Direction::right}, Animation(8, 4, 0.12f));
	animator.addAnimation({AnimationState::walking, Direction::up}, Animation(12, 4, 0.12f));
	animator.addAnimation({AnimationState::walking, Direction::down}, Animation(16, 4, 0.12f));
}

CharacterConfig Knight::defaultConfig() {
	CharacterConfig config;
	config.width = 35.0f;
	config.height = 40.0f;
	const float mapCenterX = (World::cols * World::tileSize) * 0.5f;
	const float mapCenterY = (World::rows * World::tileSize) * 0.5f;
	config.spawnX = mapCenterX - config.width * 0.5f;
	config.spawnY = mapCenterY - config.height * 0.5f;
	config.speed = 100.0f;
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/penguin/idle_down.png");
	return config;
}

std::array<std::shared_ptr<TDT4102::Image>, Knight::totalFrames> Knight::createStateDirectionSprites() {
	std::array<std::shared_ptr<TDT4102::Image>, totalFrames> loadedSprites{};
	const std::array<const char*, directionCount> directions = {"left", "right", "up", "down"};

	// Filename pattern:
	// idle: assets/characters/penguin/idle_{direction}.png
	// walk: assets/characters/penguin/walk_{direction}_{0|1|2|3}.png
    // walk: assets/characters/penguin/attack_{direction}_{0|1|2|3}.png
    // walk: assets/characters/penguin/exhausted_{direction}_{0|1|2|3}.png
    
	std::size_t index = 0;
	for (const char* directionName : directions) {
		const std::string idlePath = "assets/characters/penguin/idle_" + std::string(directionName) + ".png";
		loadedSprites[index++] = std::make_shared<TDT4102::Image>(idlePath);
	}

	for (const char* directionName : directions) {
		for (std::size_t frame = 0; frame < walkFramesPerDirection; ++frame) {
			const std::string walkPath = "assets/characters/penguin/walk_" + std::string(directionName) + "_" + std::to_string(frame) + ".png";
			loadedSprites[index++] = std::make_shared<TDT4102::Image>(walkPath);
		}
	}

	return loadedSprites;
}

InputState Knight::readInput(const TDT4102::AnimationWindow& window) const {
    InputState input;
	input.left = window.is_key_down(KeyboardKey::A);
	input.right = window.is_key_down(KeyboardKey::D);
	input.up = window.is_key_down(KeyboardKey::W);
	input.down = window.is_key_down(KeyboardKey::S);
	return input;
}

AnimationKey Knight::determineAnimationKey(const InputState& inputState) {
	AnimationKey key = lastDirectionKey;
	key.state = AnimationState::idle;

	if (inputState.left) {
		key.direction = Direction::left;
		key.state = AnimationState::walking;
	}

	if (inputState.right) {
		key.direction = Direction::right;
		key.state = AnimationState::walking;
	}

	if (inputState.up) {
		key.direction = Direction::up;
		key.state = AnimationState::walking;
	}

	if (inputState.down) {
		key.direction = Direction::down;
		key.state = AnimationState::walking;
	}

	lastDirectionKey = key;
	return key;
}

void Knight::applyAnimationFrame(int frame) {
	setSprite(sprites.at(static_cast<std::size_t>(frame)));
}
