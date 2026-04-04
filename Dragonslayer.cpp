
#include <Dragonslayer.h>
#include <Character.h>
#include <World.h>
#include <Camera.h>

void Dragonslayer::run() {
    TDT4102::AnimationWindow window;
    Camera camera(window);
    World world(window);
    Character knight(window, world);

    world.generate();
    
    while (!window.should_close()) {
        camera.follow(knight);
        knight.updatePosition();
        world.draw(camera);
        knight.draw(camera);
        window.next_frame();
    }
}