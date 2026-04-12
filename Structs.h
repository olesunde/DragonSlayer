
#pragma once

#include <Enums.h>
#include <memory>
#include <tuple>
#include "AnimationWindow.h"

struct InputState {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool attack = false;
};

struct AnimationKey {
    AnimationState state;
    Direction direction;

    bool operator<(const AnimationKey& other) const {
        return std::tie(state, direction) < std::tie(other.state, other.direction);
    }

    bool operator==(const AnimationKey& other) const {
        return state == other.state && direction == other.direction;
    }
};

struct CharacterConfig {
    float spawnX = 0.0f;
    float spawnY = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    float speed = 0.0f;
    float health = 0.0f;
    float maxHealth = 0.0f;
    float attackRange = 0.0f;
    float attackDamage = 0.0f;
    float attackCooldown = 0.0f;
    float attackWidth = 0.0f;
    float attackHeight = 0.0f;
    std::shared_ptr<TDT4102::Image> attackEffect;
    std::shared_ptr<TDT4102::Image> sprite;
    std::shared_ptr<TDT4102::Audio> soundEffect;
};