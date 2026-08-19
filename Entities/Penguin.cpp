#include "Penguin.h"
#include "AnimationWindow.h"
#include <Camera.h>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>

Penguin::Penguin()
	: Character(defaultConfig()),
	  sprites(createStateDirectionSprites()) {

	animator.addAnimation({AnimationState::idle, Direction::left}, Animation(0, 1, 0.12f)); // Finne en mer effektiv måte for initialisering
	animator.addAnimation({AnimationState::idle, Direction::right}, Animation(1, 1, 0.12f));
	animator.addAnimation({AnimationState::idle, Direction::up}, Animation(2, 1, 0.12f));
	animator.addAnimation({AnimationState::idle, Direction::down}, Animation(3, 1, 0.12f));

	animator.addAnimation({AnimationState::walking, Direction::left}, Animation(4, 4, 0.12f));
	animator.addAnimation({AnimationState::walking, Direction::right}, Animation(8, 4, 0.12f));
	animator.addAnimation({AnimationState::walking, Direction::up}, Animation(12, 4, 0.12f));
	animator.addAnimation({AnimationState::walking, Direction::down}, Animation(16, 4, 0.12f));
}

CharacterConfig Penguin::defaultConfig() {
	CharacterConfig config;
	std::map<std::string, float> values;
	const std::filesystem::path configPath = std::filesystem::path("Configs") / "penguin.txt";
	std::ifstream file(configPath);
	if (file) {
		std::string line;
		while (std::getline(file, line)) {
			const std::size_t pos = line.find('=');
			if (pos == std::string::npos) {
				continue;
			}

			const std::string key = line.substr(0, pos);
			const std::string value = line.substr(pos + 1);
			try {
				values[key] = std::stof(value);
			} catch (const std::invalid_argument&) {
				std::cerr << value << " kan ikke tolkes som en float" << std::endl;
			}
		}
	}

	config.width = values["width"];
	config.height = values["height"];
	config.spawnX = values["spawnX"];
	config.spawnY = values["spawnY"];
	config.speed = values["speed"];
	config.health = values["health"];
	config.maxHealth = values["maxHealth"];
	config.attackRange = values["attackRange"];
	config.attackDamage = values["attackDamage"];
	config.attackCooldown = values["attackCooldown"];
	config.damageCooldown = values["damageCooldown"];
    config.attackWidth = values["attackWidth"];
    config.attackHeight = values["attackHeight"];
	config.attackEffect = std::make_shared<TDT4102::Image>("assets/characters/penguin/attack_effect.png");
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/penguin/idle_down.png");
    config.soundEffect = std::make_shared<TDT4102::Audio>("assets/Audio/penguin.wav");
	return config;
}

std::array<std::shared_ptr<TDT4102::Image>, Penguin::totalFrames> Penguin::createStateDirectionSprites() {
	std::array<std::shared_ptr<TDT4102::Image>, totalFrames> loadedSprites;
	const std::array<const char*, directionCount> directions = {"left", "right", "up", "down"};
    
	std::size_t index = 0;
	for (const char* dir : directions) {
		const std::string idlePath = "assets/characters/penguin/idle_" + std::string(dir) + ".png";
		loadedSprites[index++] = std::make_shared<TDT4102::Image>(idlePath);
	}

	for (const char* dir : directions) {
		for (std::size_t frame = 0; frame < walkFramesPerDirection; ++frame) {
			const std::string walkPath = "assets/characters/penguin/walk_" + std::string(dir) + "_" + std::to_string(frame) + ".png";
			loadedSprites[index++] = std::make_shared<TDT4102::Image>(walkPath);
		}
	}
	return loadedSprites;
}

InputState Penguin::readInput(float dt, const TDT4102::AnimationWindow& window) {
    (void)dt;
    InputState input;
	input.left = window.is_key_down(KeyboardKey::A);
	input.right = window.is_key_down(KeyboardKey::D);
	input.up = window.is_key_down(KeyboardKey::W);
	input.down = window.is_key_down(KeyboardKey::S);
	const bool attackDown = window.is_key_down(KeyboardKey::SPACE);
	input.attack = attackDown && !attackWasDown;
	attackWasDown = attackDown;
	return input;
}

void Penguin::drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const {
	(void)camera;
	constexpr int barWidth = 260;
	constexpr int barHeight = 22;

	const int topX = 70;
	const int topY = window.height() - barHeight - 20;

    const int fillWidth = static_cast<int>(static_cast<float>(barWidth) * static_cast<float>(getHealthRatio()));
    if (fillWidth > 0) {
        window.draw_rectangle({topX, topY}, fillWidth, barHeight, TDT4102::Color::dark_yellow);
    }

    const std::string healthBarPath = "assets/healthBar/penguin.png";
    std::unique_ptr<TDT4102::Image> healthImage = std::make_unique<TDT4102::Image>(healthBarPath);
	window.draw_image({topX-50, topY-12}, *healthImage, 45, 40);
}

AnimationKey Penguin::determineAnimationKey(const InputState& inputState) {
	AnimationKey key = lastAnimationKey;
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

	lastAnimationKey = key;
	return key;
}

void Penguin::applyAnimationFrame(int frame) {
	setSprite(sprites.at(static_cast<std::size_t>(frame)));
}

void Penguin::setVictoryPose() {
	const std::string penguinHeart = "assets/characters/penguin/heart.png";
	setSprite(std::make_shared<TDT4102::Image>(penguinHeart));
}
