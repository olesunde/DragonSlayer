#include <Dragon.h>
#include "AnimationWindow.h"
#include <Camera.h>
#include <World.h>
#include <memory>
#include <array>
#include <string>
#include <cmath>

Dragon::Dragon()
	: Character(defaultConfig()),
	  sprites{std::make_shared<TDT4102::Image>("assets/characters/dragon/left.png"),
	         std::make_shared<TDT4102::Image>("assets/characters/dragon/right.png")},
	  deadSprites{std::make_shared<TDT4102::Image>("assets/characters/dragon/dead_left.png"),
	              std::make_shared<TDT4102::Image>("assets/characters/dragon/dead_right.png")},
	  spawnSprites{std::make_shared<TDT4102::Image>("assets/characters/dragon/spawn_0.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/dragon/spawn_1.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/dragon/spawn_2.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/dragon/spawn_3.png")},
	  eatingSprites{std::make_shared<TDT4102::Image>("assets/characters/dragon/eating_left.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/dragon/eating_right.png")} {}

CharacterConfig Dragon::defaultConfig() {
	CharacterConfig config;
	config.width = 120.0f;
	config.height = 70.0f;
	const float mapCenterX = (World::cols * World::tileSize) * 0.8f;
	const float mapCenterY = (World::rows * World::tileSize) * 0.8f;
	config.spawnX = mapCenterX - config.width * 0.5f;
	config.spawnY = mapCenterY - config.height * 0.5f;
	config.speed = 220.0f;
	config.health = 100.0f;
	config.maxHealth = 1000.0f;
	config.attackRange = 100.0f;
	config.attackDamage = 17.0f;
	config.attackCooldown = 0.0f;
    config.attackWidth = 100.0f;
    config.attackHeight = 100.0f;
	config.attackEffect = std::make_shared<TDT4102::Image>("assets/characters/dragon/attack_effect.png");
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/dragon/left.png");
    config.soundEffect = std::make_shared<TDT4102::Audio>("assets/Audio/dragon.mp3");
	return config;
}

void Dragon::setTarget(const Character& knight) {
	target = &knight;
}

void Dragon::draw(TDT4102::AnimationWindow& window, const Camera& camera) const {
	if (isDead()) {
		const std::size_t deadIndex = getDirection() == Direction::right ? 1u : 0u;
		const std::shared_ptr<TDT4102::Image>& deadSprite = deadSprites[deadIndex];
		const TDT4102::Point topLeft = camera.worldToCamera(getX(), getY());
		window.draw_image(topLeft, *deadSprite, static_cast<int>(getWidth()), static_cast<int>(getHeight()));
		return;
	}

	if (target && target->isDead()) {
		const std::size_t eatingIndex = getDirection() == Direction::right ? 1u : 0u;
		const std::shared_ptr<TDT4102::Image>& eatingSprite = eatingSprites[eatingIndex];
        const TDT4102::Point topLeft = camera.worldToCamera(getX(), getY());
		window.draw_image(topLeft, *eatingSprite, static_cast<int>(getWidth()), static_cast<int>(getHeight()));
		return;
	}

	Character::draw(window, camera);
}

InputState Dragon::readInput(float dt, const TDT4102::AnimationWindow& window) {
	(void)window;
	InputState input;

	if (isSpawning || target == nullptr || target->isDead()) {
		return input;
	}

	const double dx = target->getCenterX() - getCenterX();
	const double dy = target->getCenterY() - getCenterY();

	constexpr float idleDuration = 5.0f;
	constexpr float moveDuration = 2.5f;
	constexpr float cycleDuration = idleDuration + moveDuration;

    movementTimer += dt;
	if (movementTimer >= cycleDuration) {
		movementTimer -= cycleDuration;
    }

	const bool isMovingPhase = movementTimer >= idleDuration;

	if (isMovingPhase) {
		if (std::fabs(dx) > 1.0) {
			input.right = dx > 0.0;
			input.left = dx < 0.0;
		}

		if (std::fabs(dy) > 1.0) {
			input.down = dy > 0.0;
			input.up = dy < 0.0;
		}
		return input;
	}

	input.attack = true;

	return input;
}

void Dragon::drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const {
	(void)camera;
	constexpr int barWidth = 260;
	constexpr int barHeight = 22;
	constexpr int marginUpper = 30;

	const int topY = marginUpper;
	const int topX = window.width() * 0.25f;

    const int fillWidth = static_cast<int>(static_cast<float>(barWidth) * static_cast<float>(getHealthRatio()));
    if (fillWidth > 0) {
        window.draw_rectangle({topX, topY}, fillWidth, barHeight, TDT4102::Color::dark_red);
    }

    const std::string healthBarPath = "assets/healthBar/dragon.png";
    std::unique_ptr<TDT4102::Image> healthImage = std::make_unique<TDT4102::Image>(healthBarPath);
	window.draw_image({topX-12, marginUpper}, *healthImage, 45, 40);
}

AnimationKey Dragon::determineAnimationKey(const InputState& inputState) {
	AnimationKey key = lastAnimationKey;
	key.state = AnimationState::idle;
    
    if (inputState.left) {
		key.direction = Direction::left;
	}

	if (inputState.right) {
		key.direction = Direction::right;
	}

	lastAnimationKey = key;
	return key;
}

void Dragon::updateAnimation(float dt, const AnimationKey& key) {
	if (isSpawning) {
		spawnTimer += dt;
		const std::size_t spawnIndex = std::min(
			static_cast<std::size_t>(spawnTimer / spawnFrameDuration),
			spawnFrames - 1
		);
		setSprite(spawnSprites[spawnIndex]);

		if (spawnTimer >= spawnFrameDuration * static_cast<float>(spawnFrames)) {
			isSpawning = false;
			spawnTimer = 0.0f;
		}
		return;
	}

	if (key.direction == Direction::right) {
		setSprite(sprites[1]);
		return;
	}

	setSprite(sprites[0]);
}

void Dragon::applyAnimationFrame(int frame) {
	(void)frame;
}
