#include "AnimationWindow.h"
#include <Test.h>
#include <memory>

void Test::test() {
    TDT4102::AnimationWindow window;

    int x = 500;   // spillerens verdensposisjon
    int y = 500;
    int step = 4;

    const int playerSize = 50;
    const int windowWidth = 1000;
    const int windowHeight = 1000;

    std::unique_ptr<TDT4102::Image> image = std::make_unique<TDT4102::Image>("assets/characters/penguin/attackEffect_down.png");

    while (!window.should_close()) {
        // Flytt spilleren
        if (window.is_key_down(KeyboardKey::LEFT)) {
            x -= step;
        }
        if (window.is_key_down(KeyboardKey::RIGHT)) {
            x += step;
        }
        if (window.is_key_down(KeyboardKey::UP)) {
            y -= step;
        }
        if (window.is_key_down(KeyboardKey::DOWN)) {
            y += step;
        }

        // Kamera følger spilleren:
        // vi vil ha spilleren i midten av skjermen
        int cameraX = x - windowWidth / 2 + playerSize / 2;
        int cameraY = y - windowHeight / 2 + playerSize / 2;

        // Tegn svart firkant (spilleren)
        window.draw_image({x - cameraX, y - cameraY}, *image, playerSize, playerSize, 45.0f, {playerSize/2, playerSize/2});

        // Tegn en annen firkant i verden
        window.draw_rectangle(
            {500 - cameraX, 500 - cameraY}, 50, 50);

        window.next_frame();
    }
}