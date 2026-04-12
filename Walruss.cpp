#include <Walruss.h>
#include "AnimationWindow.h"
#include <Camera.h>
#include <World.h>
#include <memory>
#include <array>
#include <string>
#include <cmath>

Walruss::Walruss()
	: Character(defaultConfig()),
	  sprites{std::make_shared<TDT4102::Image>("assets/characters/walruss/left.png"),
	          std::make_shared<TDT4102::Image>("assets/characters/walruss/right.png")},
	  deadSprites{std::make_shared<TDT4102::Image>("assets/characters/walruss/dead_left.png"),
	              std::make_shared<TDT4102::Image>("assets/characters/walruss/dead_right.png")},
	  spawnSprites{std::make_shared<TDT4102::Image>("assets/characters/walruss/spawn_0.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/walruss/spawn_1.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/walruss/spawn_2.png"),
	               std::make_shared<TDT4102::Image>("assets/characters/walruss/spawn_3.png")},
	  eatingSprites{std::make_shared<TDT4102::Image>("assets/characters/walruss/eating_left.png"),
                   std::make_shared<TDT4102::Image>("assets/characters/walruss/eating_right.png")} {}

CharacterConfig Walruss::defaultConfig() {
	CharacterConfig config;
	config.width = 120.0f;
	config.height = 70.0f;
	const float mapCenterX = (World::cols * World::tileSize) * 0.8f;
	const float mapCenterY = (World::rows * World::tileSize) * 0.8f;
	config.spawnX = mapCenterX - config.width * 0.5f;
	config.spawnY = mapCenterY - config.height * 0.5f;
	config.speed = 60.0f;
	config.health = 100.0f;
	config.maxHealth = 100.0f;
	config.attackRange = 100.0f;
	config.attackDamage = 17.0f;
	config.attackCooldown = 2.0f;
    config.attackWidth = 100.0f;
    config.attackHeight = 100.0f;
	config.attackEffect = std::make_shared<TDT4102::Image>("assets/characters/walruss/attack_effect.png");
	config.sprite = std::make_shared<TDT4102::Image>("assets/characters/walruss/left.png");
    config.soundEffect = std::make_shared<TDT4102::Audio>("assets/Audio/walrus.mp3");
	return config;
}

void Walruss::setTarget(const Character& knight) {
	target = &knight;
}

void Walruss::draw(TDT4102::AnimationWindow& window, const Camera& camera) const {
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

InputState Walruss::readInput(float dt, const TDT4102::AnimationWindow& window) {
	(void)window;
	InputState input;

	if (isSpawning) {
		return input;
	}

	const double dx = target->getCenterX() - getCenterX();
	const double dy = target->getCenterY() - getCenterY();
    const double distance = std::sqrt(dx * dx + dy * dy);
    const float windup = 0.25f;

	if (std::fabs(dx) > 1.0f) {
		input.right = dx > 0.0f;
		input.left = dx < 0.0f;
	}

	if (std::fabs(dy) > 1.0f) {
		input.down = dy > 0.0f;
		input.up = dy < 0.0f;
	}

	if (distance <= getAttackRange()) {
        windupTimer += dt;
        if (windupTimer >= windup) {
            windupTimer = 0.0f;
            input.attack = true;

        }
	}

	return input;
}

void Walruss::drawHealthBar(TDT4102::AnimationWindow& window, const Camera& camera) const {
	constexpr int barWidth = 100;
	constexpr int barHeight = 8;
	constexpr int offsetY = 16;

	const int topX = static_cast<int>(camera.worldToCamera(getX(), getY()).x) + (static_cast<int>(getWidth()) - barWidth) / 2;
	const int topY = static_cast<int>(camera.worldToCamera(getX(), getY()).y) - barHeight - offsetY;

    const int fillWidth = static_cast<int>(static_cast<float>(barWidth) * static_cast<float>(getHealthRatio()));
    if (fillWidth > 0) {
        window.draw_rectangle({topX, topY}, fillWidth, barHeight, TDT4102::Color::dark_red);
    }
}

AnimationKey Walruss::determineAnimationKey(const InputState& inputState) {
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

void Walruss::updateAnimation(float dt, const AnimationKey& key) {
	if (isSpawning) {
		spawnTimer += dt;
		const std::size_t spawnIndex = std::min(
            static_cast<std::size_t>(spawnTimer / spawnFrameDuration), spawnFrames - 1
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

void Walruss::applyAnimationFrame(int frame) {
	(void)frame;
}
