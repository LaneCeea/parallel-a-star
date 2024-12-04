#include "Grid.h"
#include "AStar.h"
#include "Timer.h"

#include <memory>
#include <random>
#include <iostream>

int main() {
    size_t Rows = 10000;
    size_t Cols = 10000;
    std::shared_ptr<Grid> Test = std::make_shared<Grid>(Rows, Cols);
    std::cout << "Init Grid...\n";
    {
        Timer Scope("Init Grid");
        std::mt19937 Engine(std::random_device{}());
        std::bernoulli_distribution Dist(0.2);
        for (size_t i = 1; i < Rows * Cols - 1; ++i) {
            if (Dist(Engine)) {
                Test->At(i).State = CellState::OBSTACLE;
            }
        }
    }
    std::cout << "Search Start!\n";
    bool success = false;
    AStarSolver Solver(Test, GridCoord(0, 0), GridCoord(Cols - 1, Rows - 1));
    {
        Timer Scope("Search");
        success = Solver.Search();
    }
    if (success) {
        std::cout << "Search End! Path found.\n";
    } else {
        std::cout << "Search End! No path found.\n";
    }
    // Solver.PrintPath();
}