#pragma once

#include "Vec2.h"

#include <vector>
#include <cstddef>

enum class CellState {
    EMPTY = 0,
    OBSTACLE,
    OPEN,
    CLOSED,
};

struct Cell {
    CellState State;

    Cell() : State(CellState::EMPTY) {}
    Cell(CellState State) : State(State) {}
};

using GridCoord = Vec2<ptrdiff_t>;

// 2D array interface, with underlying 1D data storage
class Grid {
public:
    Grid(size_t Rows, size_t Cols) : m_Cells(Cols * Rows), m_Rows(Rows), m_Cols(Cols) {}

    void Print() const;

    Cell& operator()(ptrdiff_t Offset)                         { return m_Cells[Offset]; }
    Cell& operator()(ptrdiff_t Row, ptrdiff_t Col)                { return m_Cells[Col + Row * m_Cols]; }
    Cell& operator()(const GridCoord& Coord)             { return this->operator()(Coord.y, Coord.x); }
    const Cell& operator()(ptrdiff_t Offset) const             { return m_Cells[Offset]; }
    const Cell& operator()(ptrdiff_t Row, ptrdiff_t Col) const    { return m_Cells[Col + Row * m_Cols]; }
    const Cell& operator()(const GridCoord& Coord) const { return this->operator()(Coord.y, Coord.x); }
    
    size_t Rows() const { return m_Rows; }
    size_t Cols() const { return m_Cols; }
    size_t Size() const { return m_Rows * m_Cols; }

private:
    std::vector<Cell> m_Cells;
    size_t m_Rows;
    size_t m_Cols;
};
