import numpy as np
import time
import tkinter as tk
from lib.Core import Grid, GridCoord, CellState, AStarSolver

class FPSApp:
    def __init__(self, root):
        self.root = root

        # Initialize variables to track FPS
        self.last_time = time.time()
        self.frame_count = 0
        self.fps = 0

        # Create a label to display FPS
        self.fps_label = tk.Label(root, text="FPS: 0", font=("Helvetica", 12))
        self.fps_label.pack(pady=20)

        # Start the frame update loop
        self.update_frame()

    def update_frame(self):
        # Calculate the time elapsed since the last frame
        current_time = time.time()
        delta_time = current_time - self.last_time
        self.frame_count += 1

        # Update FPS every second
        if delta_time >= 1.0:
            self.fps = self.frame_count / delta_time
            self.fps_label.config(text=f"FPS: {self.fps:.2f}")
            self.last_time = current_time
            self.frame_count = 0

        # Schedule the next frame update
        self.root.after(1, self.update_frame)

# Grid dimensions and initial obstacles
rows, cols = 330, 330

# Initialize Tkinter window
root = tk.Tk()
root.title("A* Pathfinding Visualization")

cell_size = 3
canvas_width = cols * cell_size
canvas_height = rows * cell_size

canvas = tk.Canvas(root, width=canvas_width, height=canvas_height)
canvas.pack()

# Create and configure the grid
grid = Grid(rows, cols)
for y in range(rows):
    for x in range(cols):
        random_bool = np.random.binomial(1, 0.2)
        if (random_bool):
            grid.at(y, x).state = CellState.OBSTACLE

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

            canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="")
    draw_start_goal()

# Function to draw the solution path
def draw_solution():
    for coord in solution_closed:
        row, col = coord.y, coord.x
        x1 = col * cell_size
        y1 = row * cell_size
        x2 = x1 + cell_size
        y2 = y1 + cell_size
        canvas.create_rectangle(x1, y1, x2, y2, fill="#40D697", outline="")
    for coord in solution_open:
        row, col = coord.y, coord.x
        x1 = col * cell_size
        y1 = row * cell_size
        x2 = x1 + cell_size
        y2 = y1 + cell_size
        canvas.create_rectangle(x1, y1, x2, y2, fill="#147D5B", outline="")
    for coord in solution_path:
        row, col = coord.y, coord.x
        x1 = col * cell_size
        y1 = row * cell_size
        x2 = x1 + cell_size
        y2 = y1 + cell_size
        canvas.create_rectangle(x1, y1, x2, y2, fill="#00FF70", outline="")
    draw_start_goal()

def draw_start_goal():
    if start_node:
        x1 = start_node.x * cell_size
        y1 = start_node.y * cell_size
        x2 = x1 + cell_size
        y2 = y1 + cell_size
        canvas.create_rectangle(x1, y1, x2, y2, fill="#FF0000", outline="")
    if goal_node:
        x1 = goal_node.x * cell_size
        y1 = goal_node.y * cell_size
        x2 = x1 + cell_size
        y2 = y1 + cell_size
        canvas.create_rectangle(x1, y1, x2, y2, fill="#FF00EE", outline="")


# Function to activate the solver and display the solution
def activate_solver(event=None):
    global solution_displayed, solution_path, solution_open, solution_closed
    if not solution_displayed and start_node and goal_node:
        solver = AStarSolver(grid, start_node, goal_node)
        solver.search()
        solution_path = solver.get_solution()
        solution_open = solver.get_openset()
        solution_closed = solver.get_closedset()
        draw_solution()
        solution_displayed = True

# Function to clear the solution
def clear_solution():
    global solution_displayed, solution_path
    if solution_displayed:
        solution_displayed = False
        draw_grid()

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

app = FPSApp(root)

# Run the Tkinter event loop
root.mainloop()
