
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
    float spawnX = 0.f;
    float spawnY = 0.f;
    float width = 0.f;
    float height = 0.f;
    float speed = 0.f;
    std::shared_ptr<TDT4102::Image> sprite;
};