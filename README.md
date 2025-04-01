
# Maze-Solver

A **C++** maze generation and solving program built using the OneLoneCoder Game Engine for rendering.
This project features a **Depth-First Search (DFS) algorithm** for creating a maze and a **Breadth-First Search (BFS) algorithm** for solving it.

## Preview


https://github.com/user-attachments/assets/50579423-fa5a-4ccc-8622-2547fe841414


## Features
**Random Maze Generation**
- Uses DFS with backtracking
- Efficient cell tracking using bitwise flags and coordinate pairs

 **Maze Solving**

- Uses BFS to find the shortest path

- Maintains a stack and a vector during traversal to backtrack and extract the shortest route

**Visualized with OneLoneCoder's Console Game Engine**

- Smooth and clear rendering of each step in both creation and solving

## Algorithm Breakdown
**Maze Generation (DFS)**
- Each cell is marked using enum/bitwise operator flags.

- Cells are stored as pairs of integers: (x, y).

- A stack is used to push explored cells.

- When no unexplored neighbors are left, the algorithm backtracks by popping from the stack.

**Maze Solving (BFS)**

- Explores the maze level-by-level to ensure the shortest path is found.

- Tracks visited nodes using a stack and a vector.

- During backtracking, unnecessary paths are removed from the vector, leaving only the shortest valid route.
