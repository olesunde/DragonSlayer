#pragma once
#include "AnimationWindow.h"

#include <vector>
#include <array>
#include <memory>


class Camera;

class World {
private:
    struct DrawTile {
        TDT4102::Point topLeft;
        std::shared_ptr<TDT4102::Image> image;
    };

    enum class Tiletype {
        GROUND,
        Count
    };

    TDT4102::AnimationWindow& window;
    std::vector<std::vector<Tiletype>> world;

    std::vector<DrawTile> drawTiles;
    std::array<std::shared_ptr<TDT4102::Image>, static_cast<std::size_t>(Tiletype::Count)> cellTextures;
    void rebuildDrawCache();

public:
    World(TDT4102::AnimationWindow& window);
    
    static constexpr int tileSize = 100; // private?
    static constexpr int rows = 50; // private?
    static constexpr int cols = 50; // private?
    
    void generate();
    void draw(const Camera& camera);

    /*
    bool isWalkable(int row, int col) const;
    */
};