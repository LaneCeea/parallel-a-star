#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstddef>
#include <memory>
#include "Vec2.h"  // Ensure this header defines the Vec2 template
#include "Grid.h"  // Ensure this header defines Grid and GridCoord
#include "AStar.h"

namespace py = pybind11;

PYBIND11_MODULE(Core, m) {
    m.doc() = "A* pathfinding algorithm bindings";

    // Bind GridCoord (Vec2<ptrdiff_t>)
    py::class_<Vec2<ptrdiff_t>>(m, "GridCoord")
        .def(py::init<>())
        .def(py::init<ptrdiff_t, ptrdiff_t>(), py::arg("x"), py::arg("y"))
        .def_readwrite("x", &Vec2<ptrdiff_t>::x)
        .def_readwrite("y", &Vec2<ptrdiff_t>::y)
        .def("__add__", [](const Vec2<ptrdiff_t>& a, const Vec2<ptrdiff_t>& b) { return a + b; })
        .def("__sub__", [](const Vec2<ptrdiff_t>& a, const Vec2<ptrdiff_t>& b) { return a - b; })
        .def("__eq__", [](const Vec2<ptrdiff_t>& a, const Vec2<ptrdiff_t>& b) { return a == b; })
        .def("__ne__", [](const Vec2<ptrdiff_t>& a, const Vec2<ptrdiff_t>& b) { return a != b; });

    // Bind CellState enum
    py::enum_<CellState>(m, "CellState")
        .value("EMPTY", CellState::EMPTY)
        .value("OBSTACLE", CellState::OBSTACLE);

    // Bind Cell struct
    py::class_<Cell>(m, "Cell")
        .def(py::init<>())
        .def(py::init<CellState>(), py::arg("state"))
        .def_readwrite("state", &Cell::State);

    // Bind Grid class
    py::class_<Grid, std::shared_ptr<Grid>>(m, "Grid")
        .def(py::init<size_t, size_t>(), py::arg("rows"), py::arg("cols"))
        .def("print", &Grid::Print)
        .def("at", py::overload_cast<ptrdiff_t>(&Grid::At), py::return_value_policy::reference)
        .def("at", py::overload_cast<ptrdiff_t, ptrdiff_t>(&Grid::At), py::return_value_policy::reference)
        .def("at", py::overload_cast<const GridCoord&>(&Grid::At), py::return_value_policy::reference)
        .def("rows", &Grid::Rows)
        .def("cols", &Grid::Cols)
        .def("size", &Grid::Size);

    // Bind AStarSolver class as previously defined
    py::class_<AStarSolver>(m, "AStarSolver")
        .def(py::init<>())
        .def(py::init<const std::shared_ptr<Grid>&, const GridCoord&, const GridCoord&>(),
             py::arg("env"), py::arg("start"), py::arg("goal"))
        .def("search", &AStarSolver::Search)
        .def("print_path", &AStarSolver::PrintPath)
        .def("set_env", &AStarSolver::SetEnv, py::arg("env"), py::arg("start"), py::arg("goal"))
        .def("clear_info", &AStarSolver::ClearInfo)
        .def("get_solution", &AStarSolver::GetSolution, py::return_value_policy::reference_internal);
}
