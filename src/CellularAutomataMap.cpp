#include "CellularAutomataMap.hpp"
#include <iostream>

CellularAutomata::CellularAutomata(int width, int height) : width(width), height(height), rng(std::random_device{}()) {
    map.resize(width, std::vector<int>(height, 0));
    }

std::vector<std::vector<int>> CellularAutomata::generateMap(int iterations, float chanceToStartAlive) {
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    // Initialize the map with random cells alive
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (distribution(rng) < chanceToStartAlive) {
                map[x][y] = true;
            }
        }
    }

    // Apply cellular automata rules for specified number of iterations
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::vector<int>> newMap(width, std::vector<int>(height, 0));

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                int aliveNeighbours = countAliveNeighbours(x, y);

                if (aliveNeighbours > 4) {
                    newMap[x][y] = true;
                } else if (aliveNeighbours < 4) {
                    newMap[x][y] = false;
                } else {
                    newMap[x][y] = map[x][y];
                }
            }
        }

        map = newMap;
    }

    return map;
}

int CellularAutomata::countAliveNeighbours(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int neighbourX = x + i;
            int neighbourY = y + j;
            if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height) {
                if (i != 0 || j != 0) {
                    count += map[neighbourX][neighbourY] ? 1 : 0;
                }
            } else {
                // Edge of map, count as alive for simplicity
                ++count;
            }
        }
    }
    return count;
}

void CellularAutomata::printMap() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (map[x][y] ? "#" : ".");
        }
        std::cout << std::endl;
    }
}
