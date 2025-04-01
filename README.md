# maze-solver
Maze creator/solver program created utilising the OneLoneCoderGameEngine to render graphics.

Uses a depth-first search (DFS) algorithm to create a random maze, during creation explored cells (using enum/bitwise operator flags to mark visited cells and pairs of ints to represent cells) are pushed to a stack and backtracks when there aren't any exporable directions by pulling from the stack.

Solving utilises breadth-first search (BFS) to traverse the created maze similarly to how the maze is created, during exploration locations are stored to both a stack and a vector, backtracked cells are removed from the vector leaving the shortest route to the end goal.
