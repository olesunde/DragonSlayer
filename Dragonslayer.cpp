
#include <Dragonslayer.h>
#include <Character.h>
#include <Knight.h>
#include <Walruss.h>
#include <World.h>
#include <Camera.h>
#include <memory>
#include <vector>
#include <Timer.h>

void Dragonslayer::run() {
    Timer timer;
    TDT4102::AnimationWindow window;
    window.setBackgroundColor(TDT4102::Color::black);
    Camera camera;
    World world;

    std::vector<std::unique_ptr<Character>> characters;
    auto knight = std::make_unique<Knight>();
    auto walruss = std::make_unique<Walruss>();
    walruss->setTarget(*knight);
    characters.push_back(std::move(knight));
    characters.push_back(std::move(walruss));

    world.generate();
    
    while (!window.should_close()) {
        float dt = timer.restart();
        camera.follow(window, *characters.front());

        for (const std::unique_ptr<Character>& character : characters) {
            character->update(dt, window, world);  
        }
        
        world.draw(window, camera);
        
        for (const std::unique_ptr<Character>& character : characters) {
            character->draw(window, camera);
        }

        
        window.next_frame();
    }
}