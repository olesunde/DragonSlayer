
#include <World.h>
#include <Camera.h>

World::textureArray World::cellTextures = {
    std::make_shared<TDT4102::Image>("assets/cellTextures/blod.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/ice.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/left_edge.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/right_edge.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/upper_edge.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/lower_edge.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/rock_with_grass.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/rock.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/skeleton.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/snow_rocks.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/snow.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/lower_right_corner.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/lower_left_corner.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/upper_right_corner.png"),
    std::make_shared<TDT4102::Image>("assets/cellTextures/upper_left_corner.png")

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
    world.assign(rows, std::vector<Tiletype>(cols, Tiletype::snow));

    for (int col = 0; col < cols-1; ++col) {
        world[0][col] = Tiletype::upper_edge;
        world[rows - 1][col] = Tiletype::lower_edge;
    }

    for (int row = 0; row < rows-1; ++row) {
        world[row][0] = Tiletype::left_edge;
        world[row][cols - 1] = Tiletype::right_edge;
    }

    world[0][0] = Tiletype::upper_left_corner;
    world[0][12] = Tiletype::upper_right_corner;
    world[6][0] = Tiletype::lower_left_corner;
    world[6][12] = Tiletype::lower_right_corner;

    world[3][5] = Tiletype::blod;
    world[2][2] = Tiletype::snow_rocks;
    world[4][8] = Tiletype::skeleton;
    world[5][1] = Tiletype::skeleton;
    world[2][10] = Tiletype::snow_rocks;
    world[3][4] = Tiletype::snow_rocks;
    world[4][7] = Tiletype::snow_rocks;

    rebuildDrawCache();
}

void World::draw(TDT4102::AnimationWindow& window, const Camera& camera) {
    for (const DrawTile& tile : drawTiles) {
        TDT4102::Point topLeft = camera.worldToCamera(static_cast<float>(tile.topLeft.x), static_cast<float>(tile.topLeft.y));
        window.draw_image(topLeft, *tile.image, tileSize, tileSize);
    }
}

bool World::isBlockedTile(int row, int col) const {
    if (row <= 0 || row >= rows - 1 || col <= 0 || col >= cols - 1) {
        return true;
    }

    const Tiletype tile = world[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
    return tile == Tiletype::rock || tile == Tiletype::rock_with_grass;
}