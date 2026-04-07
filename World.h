#pragma once
#include "AnimationWindow.h"

#include <vector>
#include <array>
#include <memory>


class Camera;

class World {
public:
    static const int tileSize = 100;
    static const int rows = 10;
    static const int cols = 10;
    
    void generate();
    void draw(TDT4102::AnimationWindow& window, const Camera& camera);
    bool isBlockedTile(int row, int col) const;

private:
    struct DrawTile {
        TDT4102::Point topLeft;
        std::shared_ptr<TDT4102::Image> image;
    };

    enum class Tiletype {
        blod,
        ice,
        left_edge,
        right_edge,
        upper_edge,
        lower_edge,
        rock_with_grass,
        rock,
        skeleton,
        snow_rocks,
        snow,
        lower_right_corner,
        lower_left_corner,
        upper_right_corner,
        upper_left_corner,
        Count
    };

    std::vector<std::vector<Tiletype>> world;

    std::vector<DrawTile> drawTiles;
    using textureArray = std::array<std::shared_ptr<TDT4102::Image>, static_cast<std::size_t>(Tiletype::Count)>;
    static textureArray cellTextures;
    void rebuildDrawCache();
};