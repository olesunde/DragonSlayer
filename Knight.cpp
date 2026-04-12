#include <Knight.h>
#include "AnimationWindow.h"
#include <Camera.h>
#include <World.h>
#include <memory>
#include <array>
#include <string>

Knight::Knight()
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

CharacterConfig Knight::defaultConfig() {
	CharacterConfig config;
	config.width = 35.0f;
	config.height = 40.0f;
	const float mapCenterX = (World::cols * World::tileSize) * 0.5f;
	const float mapCenterY = (World::rows * World::tileSize) * 0.5f;
	config.spawnX = mapCenterX * 0.5 - config.width * 0.5f;
	config.spawnY = mapCenterY * 0.5 - config.height * 0.5f;
	config.speed = 200.0f;
	config.health = 100.0f;
	config.maxHealth = 100.0f;
	config.attackRange = 90.0f;
	config.attackDamage = 28.0f;
	config.attackCooldown = 0.75f;
    config.attackWidth = 50.f;
    config.attackHeight = 50.f;
	config.attackEffect = std::make_shared<TDT4102::Image>("assets/characters/penguin/attack_effect.png");
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/penguin/idle_down");
    config.soundEffect = std::make_shared<TDT4102::Audio>("assets/Audio/penguin.wav");
	return config;
}

std::array<std::shared_ptr<TDT4102::Image>, Knight::totalFrames> Knight::createStateDirectionSprites() {
	std::array<std::shared_ptr<TDT4102::Image>, totalFrames> loadedSprites;
	const std::array<const char*, directionCount> directions = {"left", "right", "up", "down"};

	// Filename pattern:
	// idle: assets/characters/penguin/idle_{direction}.png
	// walk: assets/characters/penguin/walk_{direction}_{0|1|2|3}.png
    // walk: assets/characters/penguin/attack_{direction}_{0|1|2|3}.png
    // walk: assets/characters/penguin/exhausted_{direction}_{0|1|2|3}.png
    
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

InputState Knight::readInput(float dt, const TDT4102::AnimationWindow& window) {
    (void)dt;
    InputState input;
	input.left = window.is_key_down(KeyboardKey::A);
	input.right = window.is_key_down(KeyboardKey::D);
	input.up = window.is_key_down(KeyboardKey::W);
	input.down = window.is_key_down(KeyboardKey::S);
	input.attack = window.is_key_down(KeyboardKey::SPACE);
	return input;
}

void Knight::drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const {
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

AnimationKey Knight::determineAnimationKey(const InputState& inputState) {
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

void Knight::applyAnimationFrame(int frame) {
	setSprite(sprites.at(static_cast<std::size_t>(frame)));
}

void Knight::setVictoryPose() {
	const std::string penguinHeart = "assets/characters/penguin/heart.png";
	setSprite(std::make_shared<TDT4102::Image>(penguinHeart));
}
