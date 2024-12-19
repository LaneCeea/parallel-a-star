#include "AStar.h"

#include <algorithm>
#include <array>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <cmath>

#include "Timer.h"

bool AStarSolver::Search() {
    Timer FunctionTimer("Search", TimerMode::ReportToConsole);
    m_GValue[m_Start] = 0.0f;
    m_Open.push(_CellF(m_Start, 0.0f));
    m_OpenSet.insert(m_Start);
    bool success = true;

    for (;;) {
        if (m_Open.empty()) { // Goal was never reach
            success = false;
            break;
        }

        // open cell with lowest cost
        const auto [CurrentCoord, CurrentFValue] = m_Open.top();
        m_Open.pop();
        m_OpenSet.erase(CurrentCoord);
        if (CurrentCoord == m_Goal) {
            break;
        }

        m_ClosedSet.insert(CurrentCoord);

        for (const auto& Diff : c_NeighborDiff) {
            const GridCoord NeighborCoord = CurrentCoord + Diff;
            if (_IsOutOfBound(NeighborCoord)) {
                continue;
            }
            const Cell& Neighbor = m_Env->At(NeighborCoord);

            if (Neighbor.State == CellState::OBSTACLE) {
                continue;
            }

            const float NewCost = m_GValue[CurrentCoord] + _MovementCost(CurrentCoord, NeighborCoord);

            // to be discussed: what if the heuristic is not admissible or not consistent?

            if (!m_OpenSet.contains(NeighborCoord) && !m_ClosedSet.contains(NeighborCoord)) {
                const float FValue = NewCost + _heuristic(NeighborCoord);
                m_Open.push(_CellF(NeighborCoord, FValue));
                m_OpenSet.insert(NeighborCoord);
                m_GValue.emplace(NeighborCoord, NewCost);
                m_Parent.emplace(NeighborCoord, CurrentCoord);
            }
        }
    }

    if (success) {
        GridCoord Current = m_Goal;
        while (Current != m_Start) {
            m_Solution.push_back(Current);
            Current = m_Parent[Current];
        }
        m_Solution.push_back(m_Start);
    }

    std::cout
        << "    Path      " << m_Solution.size() << '\n'
        << "    OpenSet   " << m_OpenSet.size() << '\n'
        << "    ClosedSet " << m_ClosedSet.size() << '\n';
    return true;
}

void AStarSolver::PrintPath() const {
    std::cout << "Grid with path:\n";
    std::unordered_set<GridCoord> IsOnPath;
    for (const auto& Coord : m_Solution) {
        IsOnPath.insert(Coord);
    }

    for (size_t r = 0; r < m_Env->Rows(); ++r) {
        for (size_t c = 0; c < m_Env->Cols(); ++c) {
            if (IsOnPath.contains(GridCoord(c, r))) {
                std::cout << 'O';
                continue;
            }
            if (m_ClosedSet.contains(GridCoord(c, r))) {
                std::cout << '-';
                continue;
            }
            if (m_OpenSet.contains(GridCoord(c, r))) {
                std::cout << '/';
                continue;
            }
            const auto& Current = m_Env->At(r, c);
            if (Current.State == CellState::OBSTACLE) {
                std::cout << '#';
            } else {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
}

void AStarSolver::SetEnv(const std::shared_ptr<Grid>& Env, const GridCoord& Start, const GridCoord& Goal) {
    m_Env = Env;
    m_Start = Start;
    m_Goal = Goal;
}

void AStarSolver::ClearInfo() {
    m_Solution.clear();
    m_Open = _MinHeap{};
    m_OpenSet.clear();
    m_ClosedSet.clear();
    m_GValue.clear();
    m_Parent.clear();
}

float AStarSolver::_heuristic(const GridCoord& Current) const {
    ptrdiff_t dx = std::abs(Current.x - m_Goal.x);
    ptrdiff_t dy = std::abs(Current.y - m_Goal.y);
    return dx + dy;
}

float AStarSolver::_MovementCost(const GridCoord& From, const GridCoord& To) const {
    return 1.0f;
}

bool AStarSolver::_IsOutOfBound(const GridCoord &Coord) const {
    return (
        Coord.x < 0 || Coord.y < 0 ||
        Coord.x >= (ptrdiff_t)m_Env->Cols() || Coord.y >= (ptrdiff_t)m_Env->Rows());
}