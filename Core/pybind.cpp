#include <pybind11/pybind11.h>

#include "add.h"

PYBIND11_MODULE(Core, m) {
    m.def("add", &add, "A function that adds two number");
}
