
# Maze-Solver

A **C++** maze generation and solving program built using the OneLoneCoder Game Engine for rendering.
This project features a **Depth-First Search (DFS) algorithm** for creating a maze and a **Breadth-First Search (BFS) algorithm** for solving it.

## Preview


https://github.com/user-attachments/assets/50579423-fa5a-4ccc-8622-2547fe841414


## Features
**Random Maze Generation**
- Uses DFS with backtracking to create a fully connected random maze
- Efficient cell tracking using bitwise flags and coordinate pairs

 **Maze Solving**
- Uses BFS to find the shortest path

- Maintains a stack and a vector during traversal to backtrack and extract the shortest route

**Visualized with OneLoneCoder's Console Game Engine**

- Smooth and clear rendering of each step in both creation and solving

## Algorithm Breakdown
**Maze Generation (DFS)**
- A random cell is selected as the start point

- The algorithm uses a stack to store the current path and bitwise flags to track cell walls and visited status

- Neighbors are chosen randomly and connected by "breaking" walls

- Backtracks when there are no unvisited neighbors

**Maze Solving (BFS)**

- A stack and vector are used to track explored paths and determine the shortest route.

- As the maze is solved, visited cells are drawn in cyan.

- Backtracked dead ends are removed from the shortest path vector.

- Final shortest path is displayed in yellow.
