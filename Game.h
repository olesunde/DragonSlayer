#pragma once
#include "AnimationWindow.h"
#include <Camera.h>
#include <Enums.h>
#include <Timer.h>
#include <World.h>
#include <memory>
#include <unordered_set>
#include <vector>

class Character;
class Knight;
class Walruss;
class Dragon;

class Game {
public:
    void run();
    Game();

private:
    static constexpr int stage1KillGoal = 3;

    TDT4102::AnimationWindow window;
    Timer timer;
    Camera camera;
    World world;

    bool restartWasDown = false;
    int walrussKillCount = 0;
    Stage currentStage = Stage::stage1;
    bool stage2MusicStarted = false;
    float stage2ActiveTime = 0.0f;
    std::unordered_set<const Walruss*> countedDeadWalrusses;

    std::vector<std::shared_ptr<Character>> characters;
    std::shared_ptr<Knight> knight = nullptr;
    std::vector<std::shared_ptr<Walruss>> walrusses;
    std::shared_ptr<Dragon> dragon = nullptr;
    bool dragonSpawned = false;

    std::unique_ptr<TDT4102::Audio> stage1Music;
    std::unique_ptr<TDT4102::Audio> stage2Music;

    void restart();
    void stage1();
    void stage2();
    void render();
    void update(float dt);
    void spawnWalruss();
    void drawKillCount();
};