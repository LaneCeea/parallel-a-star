#pragma once

#include "Grid.h"
#include "Vec2.h"

#include <array>
#include <memory>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// implement hash for GridCoord so we can have
// std::unordered_map<GridCoord> and std::unordered_set<GridCoord>
namespace std {
template <>
struct hash<GridCoord> {
    std::size_t operator()(const GridCoord& Val) const noexcept {
        return std::hash<ptrdiff_t>()(Val.x ^ (Val.y << 16));
    }
};
} // namespace std

class AStarSolver {
public:
    AStarSolver() = default;
    AStarSolver(const std::shared_ptr<Grid>& Env, const GridCoord& Start, const GridCoord& Goal) :
        m_Env(Env), m_Start(Start), m_Goal(Goal) {}

    bool Search();
    void PrintPath() const;
    void SetEnv(const std::shared_ptr<Grid>& Env, const GridCoord& Start, const GridCoord& Goal);
    void ClearInfo();

    const std::vector<GridCoord>& GetSolution() const { return m_Solution; }

private:
    float _heuristic(const GridCoord& Current) const;
    float _MovementCost(const GridCoord& From, const GridCoord& To) const;
    bool _IsOutOfBound(const GridCoord &Coord) const;

private:
    struct _CellF {
        GridCoord Coord;
        float FValue;
        _CellF() = default;
        _CellF(const GridCoord& Coord, float FValue) : Coord(Coord), FValue(FValue) {}

        // Compare for a min heap
        friend bool operator< (const _CellF& Lhs, const _CellF& Rhs) { return Lhs.FValue < Rhs.FValue; }
        friend bool operator> (const _CellF& Lhs, const _CellF& Rhs) { return Rhs < Lhs; }
        friend bool operator<=(const _CellF& Lhs, const _CellF& Rhs) { return !(Lhs > Rhs); }
        friend bool operator>=(const _CellF& Lhs, const _CellF& Rhs) { return !(Lhs < Rhs); }
    };
    using _MinHeap = std::priority_queue<_CellF, std::vector<_CellF>, std::greater<_CellF>>;

private:
    std::shared_ptr<Grid> m_Env;
    GridCoord m_Start, m_Goal;
    std::vector<GridCoord> m_Solution;

    _MinHeap m_Open; // O(1) min
    std::unordered_set<GridCoord> m_OpenSet; // O(1) search
    std::unordered_set<GridCoord> m_ClosedSet;
    std::unordered_map<GridCoord, float> m_GValue;
    std::unordered_map<GridCoord, GridCoord> m_Parent;

    static constexpr std::array<GridCoord, 4> c_NeighborDiff = {
        GridCoord{ -1,  0 },
        GridCoord{  1,  0 },
        GridCoord{  0,  1 },
        GridCoord{  0, -1 }
    };
};
