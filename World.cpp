
#include <World.h>
#include <algorithm>
#include <Camera.h>

// Konstruktør med initialiseringsliste
World::World(TDT4102::AnimationWindow& window): 
    window(window), 
    cellTextures{
        std::make_shared<TDT4102::Image>("assets/cellTextures/DESERT.png")
    } {}

void World::rebuildDrawCache() {
    drawTiles.clear();
    drawTiles.reserve(static_cast<std::size_t>(rows * cols));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Tiletype tileType = world[row][col];
            
            int topLeftX = col * tileSize;
            int topLeftY = row * tileSize;

            drawTiles.push_back({
                TDT4102::Point(topLeftX, topLeftY),
                cellTextures[static_cast<std::size_t>(tileType)]
            });
        }
    }
}

void World::generate() {
    world.assign(rows, std::vector<Tiletype>(cols, Tiletype::GROUND));
    rebuildDrawCache();
}

void World::draw(const Camera& camera) {
    for (const DrawTile& tile : drawTiles) {
        TDT4102::Point topLeft = camera.worldToCamera(static_cast<float>(tile.topLeft.x), static_cast<float>(tile.topLeft.y));
        window.draw_image(topLeft, *tile.image, tileSize, tileSize);
    }
}

/*
bool World::isWalkable(int row, int col) const { // må utvides hvis vi har flere ting en WALL som vi ikke skal kunne gå gjennom.
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return false;
    }

    return world[row][col] != Tiletype::WALL;
}*/