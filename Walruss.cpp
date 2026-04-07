#include <Walruss.h>
#include "AnimationWindow.h"
#include <World.h>
#include <memory>
#include <array>
#include <string>

Walruss::Walruss()
	: Character(defaultConfig()) {}

CharacterConfig Walruss::defaultConfig() {
	CharacterConfig config;
	config.width = 80.0f;
	config.height = 80.0f;
	const float mapCenterX = (World::cols * World::tileSize) * 0.8f;
	const float mapCenterY = (World::rows * World::tileSize) * 0.8f;
	config.spawnX = mapCenterX - config.width * 0.5f;
	config.spawnY = mapCenterY - config.height * 0.5f;
	config.speed = 150.0f;
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/walruss/idle_left.png");
	return config;
}

InputState Walruss::readInput(const TDT4102::AnimationWindow& window) const {
	InputState input;
	return input;
}

void Walruss::updateAnimation(float) {
	// Walruss currently uses a static sprite.
}