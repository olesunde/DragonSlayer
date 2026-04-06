
#include <World.h>
#include <Camera.h>

World::textureArray World::cellTextures = {
    std::make_shared<TDT4102::Image>("assets/cellTextures/snow.png")
};

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

void World::draw(TDT4102::AnimationWindow& window, const Camera& camera) {
    for (const DrawTile& tile : drawTiles) {
        TDT4102::Point topLeft = camera.worldToCamera(static_cast<float>(tile.topLeft.x), static_cast<float>(tile.topLeft.y));
        window.draw_image(topLeft, *tile.image, tileSize, tileSize);
    }
}