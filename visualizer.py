import tkinter as tk
from lib.Core import Grid, GridCoord, CellState, AStarSolver

# Grid dimensions and initial obstacles
rows, cols = 9, 9
initial_obstacles = [
    (4, 4), (4, 1), (4, 2), (4, 3), (4, 5), (4, 6), (4, 7),
    (1, 4), (2, 4), (3, 4), (5, 4), (6, 4), (7, 4)
]

# Initialize Tkinter window
root = tk.Tk()
root.title("A* Pathfinding Visualization")

cell_size = 80
canvas_width = cols * cell_size
canvas_height = rows * cell_size

canvas = tk.Canvas(root, width=canvas_width, height=canvas_height)
canvas.pack()

# Create and configure the grid
grid = Grid(rows, cols)
for row, col in initial_obstacles:
    grid.at(row, col).state = CellState.OBSTACLE

# Track application state
solution_displayed = False
solution_path = []
start_node = None
goal_node = None
mode = 'obstacle'  # Modes: 'obstacle', 'start_goal', 'solution'

# Function to draw the grid
def draw_grid():
    canvas.delete("all")
    for row in range(rows):
        for col in range(cols):
            x1 = col * cell_size
            y1 = row * cell_size
            x2 = x1 + cell_size
            y2 = y1 + cell_size

            cell = grid.at(row, col)
            if cell.state == CellState.OBSTACLE:
                color = "black"
            else:
                color = "white"

            if start_node and (row, col) == (start_node.y, start_node.x):
                color = "blue"
            elif goal_node and (row, col) == (goal_node.y, goal_node.x):
                color = "purple"

            canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="gray")

# Function to draw the solution path
def draw_solution():
    for coord in solution_path:
        row, col = coord.y, coord.x
        if start_node and (row, col) == (start_node.y, start_node.x):
            continue
        if goal_node and (row, col) == (goal_node.y, goal_node.x):
            continue
        x1 = col * cell_size
        y1 = row * cell_size
        x2 = x1 + cell_size
        y2 = y1 + cell_size
        canvas.create_rectangle(x1, y1, x2, y2, fill="green", outline="gray")

# Function to activate the solver and display the solution
def activate_solver(event=None):
    global solution_displayed, solution_path
    if not solution_displayed and start_node and goal_node:
        solver = AStarSolver(grid, start_node, goal_node)
        if solver.search():
            solution_path = solver.get_solution()
            draw_solution()
            solution_displayed = True
        else:
            print("No path found.")

# Function to clear the solution
def clear_solution():
    global solution_displayed, solution_path
    if solution_displayed:
        solution_path = []
        draw_grid()
        solution_displayed = False

def on_left_click(event):
    if mode == 'obstacle' and not solution_displayed:
        toggle_cell_state(event)
    elif mode == 'start_goal' and not solution_displayed:
        set_start_goal_node(event)

def on_right_click(event):
    if mode == 'start_goal' and not solution_displayed:
        set_start_goal_node(event)

# Function to toggle cell state in obstacle mode
def toggle_cell_state(event):
    col = event.x // cell_size
    row = event.y // cell_size
    if 0 <= row < rows and 0 <= col < cols:
        cell = grid.at(row, col)
        if cell.state == CellState.OBSTACLE:
            cell.state = CellState.EMPTY
        else:
            cell.state = CellState.OBSTACLE
        draw_grid()

# Function to set start or goal node in start_goal mode
def set_start_goal_node(event):
    global start_node, goal_node
    col = event.x // cell_size
    row = event.y // cell_size
    if 0 <= row < rows and 0 <= col < cols:
        cell = grid.at(row, col)
        if cell.state == CellState.EMPTY:
            coord = GridCoord(col, row)
            if event.num == 1:  # Left click
                if start_node and start_node == coord:
                    start_node = None
                elif goal_node and goal_node == coord:
                    None # no effect
                else:
                    start_node = coord
            elif event.num == 3:  # Right click
                if goal_node and goal_node == coord:
                    goal_node = None
                elif start_node and start_node == coord:
                    None # no effect
                else:
                    goal_node = coord
            draw_grid()

# Function to switch to obstacle mode
def switch_to_obstacle_mode(event=None):
    global mode
    mode = 'obstacle'
    clear_solution()

# Function to switch to start/goal selection mode
def switch_to_start_goal_mode(event=None):
    global mode
    mode = 'start_goal'
    clear_solution()

# Bind keys and mouse events
root.bind("<Return>", activate_solver)
root.bind("<r>", switch_to_obstacle_mode)
root.bind("<s>", switch_to_start_goal_mode)
canvas.bind("<Button-1>", on_left_click)
canvas.bind("<Button-3>", on_right_click)

# Draw the initial grid
draw_grid()

# Run the Tkinter event loop
root.mainloop()
