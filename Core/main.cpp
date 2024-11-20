#include "Grid.h"
#include "AStar.h"

#include <iostream>

int main() {
    Grid Test(9, 9);
    Test(4, 4).State = CellState::OBSTACLE;
    Test(4, 1).State = CellState::OBSTACLE;
    Test(4, 2).State = CellState::OBSTACLE;
    Test(4, 3).State = CellState::OBSTACLE;
    Test(4, 5).State = CellState::OBSTACLE;
    Test(4, 6).State = CellState::OBSTACLE;
    Test(4, 7).State = CellState::OBSTACLE;
    Test(1, 4).State = CellState::OBSTACLE;
    Test(2, 4).State = CellState::OBSTACLE;
    Test(3, 4).State = CellState::OBSTACLE;
    Test(5, 4).State = CellState::OBSTACLE;
    Test(6, 4).State = CellState::OBSTACLE;
    Test(7, 4).State = CellState::OBSTACLE;
    Test.Print();
    auto Path = AStar::Search(Test, GridCoord(3, 3), GridCoord(5, 5));
    AStar::PrintPath(Test, Path);
    Test.Print();
}