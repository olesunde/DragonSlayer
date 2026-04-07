#include <Knight.h>
#include "AnimationWindow.h"
#include <World.h>
#include <memory>
#include <array>
#include <string>

Knight::Knight()
	: Character(defaultConfig()),
	  sprites(createStateDirectionSprites()) {}

CharacterConfig Knight::defaultConfig() {
	CharacterConfig config;
	config.width = 40.0f;
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
	input.left = window.is_key_down(KeyboardKey::LEFT);
	input.right = window.is_key_down(KeyboardKey::RIGHT);
	input.up = window.is_key_down(KeyboardKey::UP);
	input.down = window.is_key_down(KeyboardKey::DOWN);
	return input;
}

void Knight::updateAnimation(float dt) {
	constexpr float secondsPerFrame = 0.12f;
	animationTimer += dt;

	while (animationTimer >= secondsPerFrame) {
		animationTimer -= secondsPerFrame;
		currentWalkFrame = (currentWalkFrame + 1u) % walkFramesPerDirection;
	}

	std::size_t directionOffset = 0u;

	switch (direction) {
	case Direction::left:
		directionOffset = 0u;
		break;
	case Direction::right:
		directionOffset = 1u;
		break;
	case Direction::up:
		directionOffset = 2u;
		break;
	case Direction::down:
		directionOffset = 3u;
		break;
    }

	std::size_t spriteIndex = 0u;

	if (animationState == AnimationState::walking) {
		const std::size_t stateBase = totalIdleFrames;
		spriteIndex = stateBase + directionOffset * walkFramesPerDirection + currentWalkFrame;
	} else {
		spriteIndex = directionOffset;
	}

	setSprite(sprites[spriteIndex]);
}
