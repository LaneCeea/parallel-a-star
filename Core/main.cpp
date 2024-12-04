#include "Grid.h"
#include "AStar.h"

#include <memory>

int main() {
    std::shared_ptr<Grid> Test = std::make_shared<Grid>(9, 9);
    Test->At(4, 4).State = CellState::OBSTACLE;
    Test->At(4, 1).State = CellState::OBSTACLE;
    Test->At(4, 2).State = CellState::OBSTACLE;
    Test->At(4, 3).State = CellState::OBSTACLE;
    Test->At(4, 5).State = CellState::OBSTACLE;
    Test->At(4, 6).State = CellState::OBSTACLE;
    Test->At(4, 7).State = CellState::OBSTACLE;
    Test->At(1, 4).State = CellState::OBSTACLE;
    Test->At(2, 4).State = CellState::OBSTACLE;
    Test->At(3, 4).State = CellState::OBSTACLE;
    Test->At(5, 4).State = CellState::OBSTACLE;
    Test->At(6, 4).State = CellState::OBSTACLE;
    Test->At(7, 4).State = CellState::OBSTACLE;
    Test->Print();
    AStarSolver Solver(Test, {3, 3}, {5, 5});
    Solver.Search();
    Solver.PrintPath();
    Test->Print();
}