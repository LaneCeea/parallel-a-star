#include "AStar.h"

#include <algorithm>
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include <cmath>

namespace std {
// implement hash for GridCoord so we can have
// std::unordered_map<GridCoord> and std::unordered_set<GridCoord>
template <>
struct hash<GridCoord> {
    std::size_t operator()(const GridCoord& Val) const noexcept {
        return std::hash<ptrdiff_t>()(Val.x ^ (Val.y << 16));
    }
};
} // namespace std

namespace AStar {

constexpr std::array<GridCoord, 4> c_NeighborDiff = {
    GridCoord{ -1,  0 },
    GridCoord{  1,  0 },
    GridCoord{  0,  1 },
    GridCoord{  0, -1 }
};

class _Heap {
private:
    struct _DataTy {
        GridCoord Coord;
        float FValue;
        _DataTy() = default;
        _DataTy(const GridCoord& Coord, float FValue) : Coord(Coord), FValue(FValue) {}
    };

public:
    _Heap() = default;

private:
    // Compare for a min heap
    static bool _Comp(const _DataTy& Lhs, const _DataTy& Rhs) {
        return Lhs.FValue > Rhs.FValue;
    }

public:
    void Push(const GridCoord& Coord, float FValue) {
        m_Data.emplace_back(Coord, FValue);
        std::push_heap(m_Data.begin(), m_Data.end(), _Comp);
    }

    void Pop() {
        std::pop_heap(m_Data.begin(), m_Data.end(), _Comp);
        m_Data.pop_back();
    }

    GridCoord Min() const { 
        return m_Data.front().Coord;
    }

    bool Empty() const { 
        return m_Data.empty();
    }

private:
    std::vector<_DataTy> m_Data;
};

float _h(const GridCoord& Current, const GridCoord& Goal);
float _MovementCost(const GridCoord& From, const GridCoord& To);
bool _IsOutOfBound(const Grid& Env, const GridCoord& Coord);

std::vector<GridCoord> Search(Grid& Env, const GridCoord& Start, const GridCoord& Goal) {
    std::unordered_map<GridCoord, GridCoord> Parent;
    std::unordered_map<GridCoord, float> GValue;
    GValue[Start] = 0.0f;
    _Heap Open;
    Open.Push(Start, 0.0f);

    for (;;) {
        if (Open.Empty()) {
            // Goal was never reach
            return std::vector<GridCoord>();
        }

        // open cell with lowest cost
        const GridCoord CurrentCoord = Open.Min();
        Open.Pop();
        if (CurrentCoord == Goal) {
            break;
        }

        Env(CurrentCoord).State = CellState::CLOSED;

        for (const auto& Diff : c_NeighborDiff) {
            const GridCoord NeighborCoord = CurrentCoord + Diff;
            if (_IsOutOfBound(Env, NeighborCoord)) {
                continue;
            }
            Cell& Neighbor = Env(NeighborCoord);

            if (Neighbor.State == CellState::OBSTACLE) {
                continue;
            }

            const float NewCost = GValue[CurrentCoord] + _MovementCost(CurrentCoord, NeighborCoord);

            if (Neighbor.State == CellState::OPEN && NewCost < GValue[NeighborCoord]) {
                // found better path for neighbor
                // if the heuristic is admissible and consistent, this should never happen
                // but it might not be the case in parallel mode 
            }

            if (Neighbor.State == CellState::EMPTY) {
                Neighbor.State = CellState::OPEN;
                GValue.emplace(NeighborCoord, NewCost);
                const float FValue = NewCost + _h(NeighborCoord, Goal);
                Open.Push(NeighborCoord, FValue);
                Parent.emplace(NeighborCoord, CurrentCoord);
            }
        }
    }

    std::vector<GridCoord> Result;
    GridCoord Current = Goal;
    while (Current != Start) {
        Result.push_back(Current);
        Current = Parent[Current];
    }
    Result.push_back(Start);
    return Result;
}

void PrintPath(const Grid &Env, const std::vector<GridCoord> &Path) {
    std::cout << "Grid with path:\n";
    std::unordered_set<GridCoord> IsOnPath;
    for (const auto& Coord : Path) {
        IsOnPath.insert(Coord);
    }

    for (size_t r = 0; r < Env.Rows(); ++r) {
        for (size_t c = 0; c < Env.Cols(); ++c) {
            if (IsOnPath.count(GridCoord(c, r))) {
                std::cout << 'O';
                continue;
            }
            const auto& Current = Env(r, c);
            if (Current.State == CellState::OBSTACLE) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
}

float _h(const GridCoord& Current, const GridCoord& Goal) {
    ptrdiff_t dx = std::abs(Current.x - Goal.x);
    ptrdiff_t dy = std::abs(Current.y - Goal.y);
    return dx + dy;
}

float _MovementCost(const GridCoord& From, const GridCoord& To) {
    return 1.0f;
}

bool _IsOutOfBound(const Grid &Env, const GridCoord &Coord) {
    return (Coord.x < 0 || Coord.y < 0 || Coord.x >= (ptrdiff_t)Env.Cols() || Coord.y >= (ptrdiff_t)Env.Rows());
}

}