#include "Game.h"
#include <Character.h>
#include <Dragon.h>
#include <Penguin.h>
#include <Walruss.h>
#include <iostream>

namespace {
// Bakgrunnsmusikken distribueres ikke med kildekoden (se .gitignore), saa en
// manglende lydfil skal ikke krasje spillet - det er fullt spillbart uten lyd.
void playMusicIfAvailable(TDT4102::AnimationWindow& window, TDT4102::Audio& music) {
    static bool warned = false;
    try {
        window.play_audio(music, 1);
    } catch (const std::exception& e) {
        if (!warned) {
            std::cerr << "Bakgrunnsmusikk mangler, fortsetter uten: " << e.what() << std::endl;
            warned = true;
        }
    }
}
}

Game::Game() {
    stage1Music = std::make_unique<TDT4102::Audio>("assets/Audio/walrusFight.mp3");
    stage2Music = std::make_unique<TDT4102::Audio>("assets/Audio/dragonFight.mp3");
}

void Game::run() {
    while (!window.should_close()) {
        restart();

        while (!window.should_close()) {
            const bool restartDown = window.is_key_down(KeyboardKey::R);
            if (restartDown && !restartWasDown) {
                restartWasDown = true;
                break;
            }
            restartWasDown = restartDown;

            const float dt = timer.restart();
            camera.follow(window, *characters.front());

            const bool dragonDefeated = dragonSpawned && dragon && dragon->isDead();
            if (dragonDefeated) {
                knight->setVictoryPose();
            } else {
                update(dt);
                stage1();
                stage2();
            }

            render();
            window.next_frame();
        }
    }
}

void Game::restart() {
    window.setBackgroundColor(TDT4102::Color::black);

    walrussKillCount = 0;
    currentStage = Stage::stage1;
    stage2MusicStarted = false;
    stage2ActiveTime = 0.0f;
    countedDeadWalrusses.clear();

    characters.clear();
    walrusses.clear();
    dragon.reset();
    dragonSpawned = false;

    knight = std::make_shared<Penguin>();
    characters.push_back(knight);

    spawnWalruss();
    world.generate();
    timer.restart();
    playMusicIfAvailable(window, *stage1Music);
}

void Game::update(float dt) {
    if (currentStage == Stage::stage2) {
        stage2ActiveTime += dt;
    }

    for (const std::shared_ptr<Character>& character : characters) {
        character->update(dt, window, world);
    }
}

void Game::spawnWalruss() {
    auto walruss = std::make_shared<Walruss>();
    walruss->setTarget(*knight);
    walruss->addTarget(knight);
    knight->addTarget(walruss);
    walrusses.push_back(walruss);
    characters.push_back(walruss);
}

void Game::stage1() {
    if (currentStage != Stage::stage1) {
        return;
    }

    for (const std::shared_ptr<Walruss>& walruss : walrusses) {
        if (!walruss->isDead()) {
            continue;
        }

        if (countedDeadWalrusses.contains(walruss.get())) {
            continue;
        }

        countedDeadWalrusses.insert(walruss.get());
        ++walrussKillCount;
    }

    if (walrussKillCount >= stage1KillGoal) {
        currentStage = Stage::stage2;
        stage2ActiveTime = 0.0f;
        knight->clearTargets();
        return;
    }

    bool hasAliveWalruss = false;
    for (const std::shared_ptr<Walruss>& walruss : walrusses) {
        if (!walruss->isDead()) {
            hasAliveWalruss = true;
            break;
        }
    }

    if (!hasAliveWalruss) {
        spawnWalruss();
    }
}

void Game::stage2() {
    if (currentStage != Stage::stage2) {
        return;
    }

    if (!stage2MusicStarted) {
        playMusicIfAvailable(window, *stage2Music);
        stage2MusicStarted = true;
    }

    if (dragonSpawned) {
        return;
    }

    constexpr float dragonSpawnDelaySeconds = 17.0f;
    if (stage2ActiveTime < dragonSpawnDelaySeconds) {
        return;
    }

    dragon = std::make_shared<Dragon>();
    dragon->setTarget(*knight);
    dragon->addTarget(knight);
    knight->addTarget(dragon);
    characters.push_back(dragon);
    dragonSpawned = true;
}

void Game::drawKillCount() {
    constexpr int topLeftX = 20;
    constexpr int topLeftY = 20;
    constexpr int imageWidth = 60;
    constexpr int imageHeight = 80;

    switch (walrussKillCount) {
        case 0: {
            auto image = std::make_unique<TDT4102::Image>("assets/killcounts/walruss_0.png");
            window.draw_image({topLeftX, topLeftY}, *image, imageWidth, imageHeight);
            break;
        }
        case 1: {
            auto image = std::make_unique<TDT4102::Image>("assets/killcounts/walruss_1.png");
            window.draw_image({topLeftX, topLeftY}, *image, imageWidth, imageHeight);
            break;
        }
        case 2: {
            auto image = std::make_unique<TDT4102::Image>("assets/killcounts/walruss_2.png");
            window.draw_image({topLeftX, topLeftY}, *image, imageWidth, imageHeight);
            break;
        }
        default: {
            auto image = std::make_unique<TDT4102::Image>("assets/killcounts/walruss_3.png");
            window.draw_image({topLeftX, topLeftY}, *image, imageWidth, imageHeight);
            break;
        }
    }
}

void Game::render() {
    world.draw(window, camera);

    for (const std::shared_ptr<Character>& character : characters) {
        if (!character->isDead()) {
            continue;
        }
        if (character == knight) {
            continue;
        }
        character->draw(window, camera);
    }

    for (const std::shared_ptr<Character>& character : characters) {
        if (character->isDead()) {
            continue;
        }
        character->draw(window, camera);
    }

    drawKillCount();
}