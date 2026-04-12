
#include <Dragonslayer.h>
#include <Character.h>
#include <Knight.h>
#include <Walruss.h>
#include <Dragon.h>
#include <World.h>
#include <Camera.h>
#include "AnimationWindow.h"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include <Timer.h>
#include <Enums.h>

void Dragonslayer::run() {
    TDT4102::AnimationWindow window;
    constexpr int stage1KillGoal = 3;
    bool restartWasDown = false;
    auto stage1Music = std::make_unique<TDT4102::Audio>("assets/Audio/walrusFight.mp3");
    auto stage2Music = std::make_unique<TDT4102::Audio>("assets/Audio/dragonFight.mp3");

    while (!window.should_close()) {
        Timer timer;
        window.setBackgroundColor(TDT4102::Color::black);
        Camera camera;
        World world;
    
        int walrussKillCount = 0;
        Stage currentStage = Stage::stage1;
        bool stage2MusicStarted = false;
        std::unordered_set<const Walruss*> countedDeadWalrusses;

        std::vector<std::shared_ptr<Character>> characters;
        auto knight = std::make_shared<Knight>();
        characters.push_back(knight);
        std::vector<std::shared_ptr<Walruss>> walrusses;
        std::shared_ptr<Dragon> dragon = nullptr;
        bool dragonSpawned = false;

        auto spawnWalruss = [&]() {
            auto walruss = std::make_shared<Walruss>();
            walruss->setTarget(*knight);
            walruss->addTarget(knight);
            knight->addTarget(walruss);
            walrusses.push_back(walruss);
            characters.push_back(walruss);
        };

        spawnWalruss();
        world.generate();
        timer.restart();
        window.play_audio(*stage1Music, 1); //hvordan spille lenger?

        while (!window.should_close()) {
            const bool restartDown = window.is_key_down(KeyboardKey::R);
            if (restartDown && !restartWasDown) {
                restartWasDown = true;
                break;
            }
            restartWasDown = restartDown;
            
            const float dt = timer.restart();
            camera.follow(window, *characters.front());

            for (const std::shared_ptr<Character>& character : characters) {
                character->update(dt, window, world);
            }

            if (currentStage == Stage::stage1) {
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
                    knight->clearTargets();
                    if (!stage2MusicStarted) {
                        window.play_audio(*stage2Music, 1); //overlapper lyden?
                        stage2MusicStarted = true;
                    }
                } else {
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
            }

            if (currentStage == Stage::stage2 && !dragonSpawned) {
                dragon = std::make_shared<Dragon>();
                dragon->setTarget(*knight);
                dragon->addTarget(knight);
                knight->addTarget(dragon);
                characters.push_back(dragon);
                dragonSpawned = true;
            }

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

            drawKillCount(window, walrussKillCount);
            window.next_frame();
        }
    }
}

void Dragonslayer::drawKillCount(TDT4102::AnimationWindow& window, int walrussKillCount) {
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
