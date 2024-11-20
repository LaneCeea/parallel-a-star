#pragma once

#include "Grid.h"
#include "Vec2.h"

#include <array>
#include <vector>

namespace AStar {

std::vector<GridCoord> Search(Grid& Env, const GridCoord& Start, const GridCoord& Goal);

void PrintPath(const Grid& Env, const std::vector<GridCoord>& Path);

}