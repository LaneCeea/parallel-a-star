CXX := g++
CXX_FLAGS := -O3 -Wall -std=c++20
DYNAMIC_FLAGS := -shared -fPIC
PYBIND11_INCLUDE := -Iextern/pybind11/include/
PYTHON_INCLUDE := $(shell python3-config --includes)
OUTPUT_EXT := $(shell python3-config --extension-suffix)

# please modify when creating or deleting files 
CPP_FILE := Core/AStar.cpp Core/Grid.cpp
HEADER_FILE := Core/AStar.h Core/Grid.h Core/Vec2.h Core/Timer.h

PYBIND_FILE := Core/pybind.cpp
CPP_MAIN_FILE := Core/main.cpp

all: python_ext cpp_exe

python_ext: $(CPP_FILE) $(HEADER_FILE) $(PYBIND_FILE) | lib
	$(CXX) $(CXX_FLAGS) $(DYNAMIC_FLAGS) $(PYTHON_INCLUDE) $(PYBIND11_INCLUDE) $(CPP_FILE) $(PYBIND_FILE) -o lib/Core$(OUTPUT_EXT)

cpp_exe: $(CPP_FILE) $(HEADER_FILE) $(CPP_MAIN_FILE) | bin
	$(CXX) $(CXX_FLAGS) $(CPP_FILE) $(CPP_MAIN_FILE) -o bin/Core

lib:
	mkdir -p lib

bin:
	mkdir -p bin

clean:
	rm -rf bin lib
