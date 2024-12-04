#include "Grid.h"

#include <iostream>

void Grid::Print() const {
    std::cout << "Grid:\n";
    for (size_t r = 0; r < m_Rows; ++r) {
        for (size_t c = 0; c < m_Cols; ++c) {
            const auto& Current = this->At(r, c);
            switch (Current.State)
            {
            case CellState::EMPTY:
                std::cout << '.';
                break;
            case CellState::OBSTACLE:
                std::cout << '#';
                break;
            default:
                break;
            }
        }
        std::cout << '\n';
    }
}
