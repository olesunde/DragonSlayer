
#pragma once

#include <Enums.h>
#include <Structs.h>
#include "Animation.h"
#include <map>

class Animator {
private:
    std::map<AnimationKey, Animation> animations;
    AnimationKey currentKey{AnimationState::idle, Direction::down};

public:
    Animator() = default;
    void addAnimation(const AnimationKey& key, const Animation& animation);
    void play(const AnimationKey& key);
    void update(float dt);
    int getFrame() const;
};