
#include "Game.h"
#include <exception>
#include <iostream>

int main() {
    try {
        Game{}.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Programmet krasjet: " << e.what() << std::endl;
        return 1;
    }
}