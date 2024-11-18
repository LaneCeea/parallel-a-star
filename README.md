This project is aimed for Linux environment.

***

## Requirement
1. `gcc`
2. `make`
3. `python` 3.8 or newer
4. `python3-dev`

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