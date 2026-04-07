#include <Walruss.h>
#include "AnimationWindow.h"
#include <World.h>
#include <memory>
#include <array>
#include <string>
#include <cmath>

Walruss::Walruss()
	: Character(defaultConfig()),
	  sprites{std::make_shared<TDT4102::Image>("assets/characters/walruss/idle_left.png")} {
	for (Direction direction : {Direction::left, Direction::right, Direction::up, Direction::down}) {
		animator.addAnimation({AnimationState::idle, direction}, Animation(0, 1, 0.12f));
		animator.addAnimation({AnimationState::walking, direction}, Animation(0, 1, 0.12f));
	}
}

CharacterConfig Walruss::defaultConfig() {
	CharacterConfig config;
	config.width = 90.0f;
	config.height = 70.0f;
	const float mapCenterX = (World::cols * World::tileSize) * 0.8f;
	const float mapCenterY = (World::rows * World::tileSize) * 0.8f;
	config.spawnX = mapCenterX - config.width * 0.5f;
	config.spawnY = mapCenterY - config.height * 0.5f;
	config.speed = 100.0f;
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/walruss/idle_left.png");
	return config;
}

void Walruss::setTarget(const Character& knight) {
	target = &knight;
}

InputState Walruss::readInput(const TDT4102::AnimationWindow& window) const {
	(void)window;
	InputState input;

	if (target == nullptr) {
		return input;
	}

	const float dx = target->getX() - getX();
	const float dy = target->getY() - getY();
	constexpr float deadZone = 6.0f;

	if (std::fabs(dx) > deadZone) {
		input.right = dx > 0.0f;
		input.left = dx < 0.0f;
	}

	if (std::fabs(dy) > deadZone) {
		input.down = dy > 0.0f;
		input.up = dy < 0.0f;
	}

	return input;
}

AnimationKey Walruss::determineAnimationKey(const InputState& inputState) {
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

void Walruss::applyAnimationFrame(int frame) {
	setSprite(sprites.at(static_cast<std::size_t>(frame)));
}