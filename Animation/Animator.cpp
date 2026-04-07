
#include "Animator.h"

void Animator::addAnimation(const AnimationKey& key, const Animation& animation) {
    animations.insert_or_assign(key, animation);
}

void Animator::play(const AnimationKey& key) {
    if (key != currentKey) {
        currentKey = key;
        animations.at(currentKey).reset();
    }
}

void Animator::update(float dt) {
    animations.at(currentKey).update(dt);
}

int Animator::getFrame() const {
    return animations.at(currentKey).getFrame();
}

bool Animator::isFinished() const {
    return animations.at(currentKey).isFinished();
}