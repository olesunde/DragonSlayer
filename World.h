#pragma once
#include "AnimationWindow.h"

#include <vector>
#include <array>
#include <memory>


class Camera;

class World {
public:
    int tileSize = 100;
    int rows = 50;
    int cols = 50;
    
    void generate();
    void draw(TDT4102::AnimationWindow& window, const Camera& camera);

private:
    struct DrawTile {
        TDT4102::Point topLeft;
        std::shared_ptr<TDT4102::Image> image;
    };

    enum class Tiletype {
        GROUND,
        Count
    };

    std::vector<std::vector<Tiletype>> world;

    std::vector<DrawTile> drawTiles;
    using textureArray = std::array<std::shared_ptr<TDT4102::Image>, static_cast<std::size_t>(Tiletype::Count)>;
    static textureArray cellTextures;
    void rebuildDrawCache();
};