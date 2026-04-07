
#pragma once

class Animation {
public:
    Animation(int startFrame, int frameCount, float timePerFrame, bool looping=true);

    void reset();
    void update(float dt);
    int getFrame() const;
    bool isFinished() const;

private:
    int startFrame;
    int frameCount;
    float timePerFrame;
    float timer;
    int currentFrame;
    bool looping;
    bool finished;
};

