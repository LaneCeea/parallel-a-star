This project is aimed for Linux and macOS environment.

***

## Requirement
1. `gcc`
2. `make`
3. `python` 3.8 or newer
4. `python3-dev`
5. `python3-tk` (or just make sure python can import `tkinter`)

***

## Installation
```
git clone --recursive https://github.com/LaneCeea/parallel-a-star
cd parallel-a-star
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

***

## Usage
There are currently two configuration for compilation: `cpp_exe` and `python_ext`.
```
make cpp_exe
./bin/Core
```
This configuration compiles the C++ sources into a standalone executable. The executable will be located in `bin/` directory.
```
make python_ext
python3 visualizer.py
```
This configuration compiles the C++ sources into a shared library that Python can import. The library will be located in `lib/` directory.

***

## Interactive Visualizer Control
There are three modes when launching the python visualizer.
##### 1. toggel-empty-obstacle mode
This is the default mode. You can press R to enter this mode.
In this mode, press left click to mark/unmark a cell as an obstacle.
##### 2. select-start-goal mode
You can press S to enter this mode.
In this mode, press left click to select/unselect a cell as the start node, and right click to select/unselect a cell as the goal node.
##### 3. show-solution mode
You can press Enter to enter this mode if the start node and the goal node have been selected.
Upon entering this mode, the A* solver will start the search and block the process (currently). After the search is done, the process is unblocked. If a solution exists, then the solution will be shown on the grid, or else a failed message will be printed on the console. 