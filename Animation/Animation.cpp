
#include "Animation.h"

Animation::Animation(int startFrame, int frameCount, float timePerFrame, bool looping)
        : startFrame(startFrame),
          frameCount(frameCount),
          timePerFrame(timePerFrame),
          timer(0.0f),
          currentFrame(0),
          looping(looping),
          finished(false) {}

void Animation::reset() {
    timer = 0.0f;
    currentFrame = 0;
    finished = false;
}

void Animation::update(float dt) {
    if (finished) return;

    timer += dt;
    while (timer >= timePerFrame) {
        timer -= timePerFrame;
        currentFrame++;

        if (currentFrame >= frameCount) {
            if (looping) {
                currentFrame = 0;
            } else {
                currentFrame = frameCount - 1;
                finished = true;
            }
        }
    }
}

int Animation::getFrame() const {
    return startFrame + currentFrame;
}