#include <iostream>
#include <stack>
#include <set>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Maze : public olc::PixelGameEngine
{
public:
	Maze()
	{
		sAppName = "Maze";
		mazeCreated = false;
		mazeSolved = false;
	}

private:
	int mazeWidth;
	int mazeHeight;
	int* mazeArr;

	int screenWidth;
	int screenHeight;


	enum
	{
		CELL_PATH_N = 0x01,
		CELL_PATH_E = 0x02,
		CELL_PATH_S = 0x04,
		CELL_PATH_W = 0x08,
		CELL_VISITED = 0x10,
	};

	int visitedCells;
	int pathWidth;

	stack<pair<int, int>> creationStack;

	// For completing the maze
	int startingPosX, startingPosY;
	int endingPosX, endingPosY;

	stack<pair<int, int>> solutionStack;
	int solutionSteps;
	bool mazeCreated;
	bool mazeSolved;
	bool shortestRouteFound;
	set<pair<int, int>> visitedNeighbours;
	vector<pair<int, int>> shortestRoute;

protected:
	// Called once at the start, so create things here
	virtual bool OnUserCreate()
	{
		srand(time(nullptr));

		//maze parameters
		mazeWidth = 40;
		mazeHeight = 25;
		pathWidth = 3;

		mazeArr = new int[mazeWidth * mazeHeight];

		memset(mazeArr, 0x00, mazeWidth * mazeHeight * sizeof(int));



		//push the starting location of the maze generator 
		int x = rand() % mazeWidth;
		int y = rand() % mazeHeight;
		creationStack.push(make_pair(x, y));
		mazeArr[y * mazeWidth + x] = CELL_VISITED;
		visitedCells = 1;

		// For the starting/ending cells location post maze creation
		startingPosX = rand() % mazeWidth;
		startingPosY = rand() % mazeHeight;
		endingPosX = rand() % mazeWidth;
		endingPosY = rand() % mazeHeight;
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{

		if (!mazeSolved) {
			this_thread::sleep_for(5ms);
		}

		if (!mazeCreated)	// generate maze
		{
			CreateMaze();
		}
		else if (!mazeSolved)		// maze solving algorithm
		{			
			SolveMaze();
		}
		else if (!shortestRouteFound)
		{
			DrawShortestRoute();
		}

		return true;

	}

	void CreateMaze() {

		auto offset = [&](int x, int y)
			{
				return (creationStack.top().second + y) * mazeWidth + (creationStack.top().first + x);
			};

		if (visitedCells < mazeWidth * mazeHeight) {
			if (visitedCells < mazeWidth * mazeHeight)
			{
				vector<int> neighbours;

				// Search for neighbouring cells
				// North neighbour
				if (creationStack.top().second > 0 && (mazeArr[offset(0, -1)] & CELL_VISITED) == 0)	// if it isn't looking along the top row
					neighbours.push_back(0);

				// East neighbour
				if (creationStack.top().first < mazeWidth - 1 && (mazeArr[offset(1, 0)] & CELL_VISITED) == 0)	// if it isn't looking along the top row
					neighbours.push_back(1);

				// South neighbour
				if (creationStack.top().second < mazeHeight - 1 && (mazeArr[offset(0, 1)] & CELL_VISITED) == 0)	// if it isn't looking along the top row
					neighbours.push_back(2);

				// West neighbour
				if (creationStack.top().first > 0 && (mazeArr[offset(-1, 0)] & CELL_VISITED) == 0)	// if it isn't looking along the top row
					neighbours.push_back(3);


				// If there are available neighbours
				if (!neighbours.empty())
				{
					int next_cell_dir = neighbours[rand() % neighbours.size()];		// choose random direction to go

					// Create path between neighbour and current cell
					switch (next_cell_dir)
					{
					case 0: // North
						mazeArr[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
						mazeArr[offset(0, 0)] |= CELL_PATH_N;
						creationStack.push(make_pair((creationStack.top().first + 0), (creationStack.top().second - 1)));
						break;

					case 1: // East
						mazeArr[offset(1, 0)] |= CELL_VISITED | CELL_PATH_W;
						mazeArr[offset(0, 0)] |= CELL_PATH_E;
						creationStack.push(make_pair((creationStack.top().first + 1), (creationStack.top().second + 0)));
						break;

					case 2: // South
						mazeArr[offset(0, 1)] |= CELL_VISITED | CELL_PATH_N;
						mazeArr[offset(0, 0)] |= CELL_PATH_S;
						creationStack.push(make_pair((creationStack.top().first + 0), (creationStack.top().second + 1)));
						break;

					case 3: // West
						mazeArr[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
						mazeArr[offset(0, 0)] |= CELL_PATH_W;
						creationStack.push(make_pair((creationStack.top().first - 1), (creationStack.top().second + 0)));
						break;
					}

					//new cell
					mazeArr[offset(0, 0)] |= CELL_VISITED;
					visitedCells++;

				}
				else {
					creationStack.pop();	// backtrack
				}
			}

			for (int x = 0; x < mazeWidth; x++)
			{
				for (int y = 0; y < mazeHeight; y++)
				{
					for (int py = 0; py < pathWidth; py++)		//this draws in each cell to make the walls
						for (int px = 0; px < pathWidth; px++)
						{
							if (mazeArr[y * mazeWidth + x] & CELL_VISITED)
								Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, olc::WHITE);
							else
								Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, olc::BLUE);
						}

					// for drawing the path ways (through walls)
					for (int p = 0; p < pathWidth; p++)
					{
						if (mazeArr[y * mazeWidth + x] & CELL_PATH_S)
							Draw(x * (pathWidth + 1) + p, y * (pathWidth + 1) + pathWidth, olc::WHITE); // Draw South Passage
						if (mazeArr[y * mazeWidth + x] & CELL_PATH_E)
							Draw(x * (pathWidth + 1) + pathWidth, y * (pathWidth + 1) + p, olc::WHITE); // Draw East Passage
					}

				}
			}

			// for colouring the cell at the top of the stack 
			if (visitedCells < mazeWidth * mazeHeight) {
				for (int py = 0; py < pathWidth; py++)
					for (int px = 0; px < pathWidth; px++)
						Draw(creationStack.top().first * (pathWidth + 1) + px, creationStack.top().second * (pathWidth + 1) + py, olc::BLACK);
			}

		}
		else	// maze creation is completed
		{
			solutionStack.push(make_pair(startingPosX, startingPosY));
			visitedNeighbours.insert(make_pair(startingPosX, startingPosY));
			//cout << "Solution stack starts " << m_solutionStack.top().first << ", " << m_solutionStack.top().second << "\n";

			DrawCell(startingPosX, startingPosY, olc::GREEN);
			DrawCell(endingPosX, endingPosY, olc::RED);

			solutionSteps = 1;
			mazeCreated = true;
		}
	}

	void SolveMaze() {
		// Check if the maze solving is possible
		if (solutionStack.empty()) {
			//cout << "Stack is empty, maze cannot be solved!" << endl;
			return;
		}

		// Get the current cell
		int x = solutionStack.top().first;
		int y = solutionStack.top().second;

		//cout << "Current cell: (" << x << ", " << y << ")" << endl;

		auto neighbour = make_pair(x, y);
		visitedNeighbours.insert(neighbour);

		//cout << "Visited cells: ";
		/*for (auto all : visited_neighbours) {
			cout << " (" << all.first << ", " << all.second << ") ";
		}*/

		// Check if the current cell is the end point
		if (x == endingPosX && y == endingPosY) {
			//cout << "Maze solved!" << endl;
			//cout << "Solution stack size: " << m_solutionStack.size();
			mazeSolved = true;
			return;
		}

		// Store and explore neighbors
		vector<int> neighbours;

		//cout << "Steps taken: " << m_solutionSteps << endl;
		
		//cout << "Available directions:";
		//North neighbor
		if (y > 0 && (mazeArr[y * mazeWidth + x] & CELL_PATH_N)) {
			if (!ContainsPair(visitedNeighbours, x, y - 1)) {
				//cout << " up ";
				neighbours.push_back(0);
			}
		}
		//East neighbor
		if (x < mazeWidth - 1 && (mazeArr[y * mazeWidth + x] & CELL_PATH_E)) {
			if (!ContainsPair(visitedNeighbours, x + 1, y)) {
				//cout << " right ";
				neighbours.push_back(1);
			}
		}
		//South neighbor
		if (y < mazeHeight - 1 && (mazeArr[y * mazeWidth + x] & CELL_PATH_S)) {
			if (!ContainsPair(visitedNeighbours, x, y + 1)) {
				//cout << " down ";
				neighbours.push_back(2);
			}
		}
		//West neighbor
		if (x > 0 && (mazeArr[y * mazeWidth + x] & CELL_PATH_W)) {
			if (!ContainsPair(visitedNeighbours, x - 1, y)) {
				//cout << " left ";
				neighbours.push_back(3);
			}
		}

		// Check if any unvisited neighbour is available
		if (!neighbours.empty()) {
			// Choose a random direction
			int next_cell_dir = neighbours[rand() % neighbours.size()];
			//cout << "\nChosen direction:";
			// Move to the next cell based on the chosen direction
			switch (next_cell_dir) {
			case 0: // North
				solutionStack.push(make_pair(x, y - 1));
				//cout << "up" << endl;
				DrawVerticalCell(x, y, -1, olc::DARK_CYAN);
				break;
			case 1: // East
				solutionStack.push(make_pair(x + 1, y));
				//cout << "right" << endl;
				DrawHorizontalCell(x, y, 1, olc::DARK_CYAN);
				break;
			case 2: // South
				solutionStack.push(make_pair(x, y + 1));
				//cout << "down" << endl;
				DrawVerticalCell(x, y, 1, olc::DARK_CYAN);
				break;
			case 3: // West
				solutionStack.push(make_pair(x - 1, y));
				//cout << "left" << endl;
				DrawHorizontalCell(x, y, -1, olc::DARK_CYAN);
				break;
			}
			//cout << endl;
			auto neighbour = make_pair(x, y);
			solutionSteps++;
			visitedNeighbours.insert(neighbour);
			shortestRoute.push_back(neighbour);
		}
		else {
			// If no unvisited neighbour is available, backtrack
			if (ContainsPair(shortestRoute, solutionStack.top().first, solutionStack.top().second)) {
				// if this != starting cell
				auto it = std::remove(shortestRoute.begin(), shortestRoute.end(), make_pair(solutionStack.top().first, solutionStack.top().second));

				// Erase the pair from the vector
				shortestRoute.erase(it, shortestRoute.end());
			}

			solutionStack.pop();
			//cout << "Backtracked" << endl;
		}

		// Drawing in all the visited cells
		for (auto v : visitedNeighbours) {
			DrawCell(v.first, v.second, olc::DARK_CYAN);
		}

		// Drawing in the top of the stack for visibility 
		DrawCell(solutionStack.top().first, solutionStack.top().second, olc::GREEN);
		// Drawing the starting cell
		DrawCell(startingPosX, startingPosY, olc::GREEN);
		DrawCell(endingPosX, endingPosY, olc::RED);
	}

	void DrawShortestRoute() {
		// 0, 0
		int counter = 0;
		auto prev = *shortestRoute.begin();
		//cout << prev.first << " " << prev.second;

		//auto current = prev;
		for (auto current : shortestRoute) {

			// 0, 0		1, 0	(moves to right)
			if (prev.first > current.first)
				DrawHorizontalCell(current.first, current.second, 1, olc::YELLOW);

			else if (prev.first < current.first)
				DrawHorizontalCell(current.first, current.second, -1, olc::YELLOW);

			// 0, 0		0, 1	(moves up)
			else if (prev.second > current.second)
				DrawVerticalCell(current.first, current.second, 1, olc::YELLOW);

			else if (prev.second < current.second)
				DrawVerticalCell(current.first, current.second, -1, olc::YELLOW);

			counter++;
			prev = current;
			//cout << endl << prev.first << ", " << prev.second;
		}
		cout << "Steps taken to find end: " << solutionSteps << endl;
		cout << "Shortest route: " << counter;
		shortestRouteFound = true;
	}


	void DrawHorizontalCell(int x, int y, int dir, olc::Pixel colour) {
		if (dir == -1) {	//left
			for (int py = 0; py < pathWidth; py++)
			{
				for (int px = -1; px < pathWidth; px++)
				{
					Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, colour); // Draw Cell
				}
			}
		}
		else if (dir == 1) {	//right
			for (int py = 0; py < pathWidth; py++)
			{
				for (int px = 0; px < pathWidth + 1; px++)
				{
					Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, colour); // Draw Cell
				}
			}
		}
	}

	void DrawVerticalCell(int x, int y, int dir, olc::Pixel colour) {
		if (dir == -1) {	//down
			for (int py = -1; py < pathWidth; py++)
			{
				for (int px = 0; px < pathWidth; px++)
				{
					Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, colour); // Draw Cell
				}
			}
		}
		else if (dir == 1) {	//up
			for (int py = 0; py < pathWidth + 1; py++)
			{
				for (int px = 0; px < pathWidth; px++)
				{
					Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, colour); // Draw Cell
				}
			}
		}
	}

	bool ContainsPair(set<pair<int, int>>& s, int x, int y) {
		return s.find(make_pair(x, y)) != s.end();
	}

	bool ContainsPair(stack<pair<int, int>>& s, int x, int y) {
		if (s.top().first == x && s.top().second == y) {
			return true;
		}
		return false;
	}

	bool ContainsPair(const vector<pair<int, int>>& v, int x, int y) {
		for (const auto& pair : v) {
			if (pair.first == x && pair.second == y) {
				return true;
			}
		}
		return false;
	}

	void DrawCell(int x, int y, olc::Pixel colour) {
		for (int py = 0; py < pathWidth; py++)
		{
			for (int px = 0; px < pathWidth; px++)
			{
				Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, colour); // Draw Cell
			}
		}
	}
};


int main()
{
	
	Maze maze;
	maze.Construct(160, 100, 8, 8);		//160x100 chars, 8x8 pixels
	maze.Start();

	return 0;
}