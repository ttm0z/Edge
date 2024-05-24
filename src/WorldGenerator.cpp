#include "WorldGenerator.hpp"
#include "CellularAutomataMap.hpp"
#include <iostream>
#include <iomanip> // For std::setw
#include <algorithm> // For std::find

WorldGenerator::WorldGenerator() {
    //worldSize = generateWorldSize(1.2, 0.5, 0.7, 1.3);
    worldSize = 1;
    generateWorld();
}

void WorldGenerator::generateWorld() {
    
    // seed values for CA
    const int iterations = 4;
    const float chanceToStartAlive = 0.4f;
    
    // determine grid size
    grid_size = worldSize * 160;
    worldMap.resize(grid_size, std::vector<int>(grid_size, 0));

    CellularAutomata automata(grid_size, grid_size);

    worldMap = automata.generateMap(iterations, chanceToStartAlive);
}

// Function to calculate the Manhattan distance between two points
int WorldGenerator::manhattanDistance(const Cell& p1, const Cell& p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}


int WorldGenerator::getGridSize(){
    return grid_size;
}


/**
 * Generate world size selected from gaussian range 0.2m - 2.5m
*/
double WorldGenerator::generateWorldSize(double mean, double std_dev, double min_val, double max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, std_dev);

    double result;
    do {
        result = distribution(gen);
    } while (result < min_val || result > max_val);

    return result;
}

void WorldGenerator::printMap(){
    for (int i = 0; i < worldMap.size(); ++i) {
        for (int j = 0; j < worldMap[0].size(); ++j) {
            std::cout << worldMap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> WorldGenerator::getMap(){
    return worldMap;
}
