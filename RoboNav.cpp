#include <iostream>
#include <vector>
#include <climits>
#include <fstream>

using namespace std;

// Directions for moving in a grid (up, right, down, left)
const vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void printPath(const vector<pair<int, int>>& path) {
    for (const auto& p : path) {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
}

void outputPathToCSV(const vector<pair<int, int>>& path) {
    ofstream file("shortest_path.csv");

    if (file.is_open()) {
        for (const auto& p : path) {
            file << p.first << "," << p.second << "\n";
        }
        file.close();
        cout << "Shortest path written to shortest_path.csv\n";
    } else {
        cerr << "Failed to open file for writing.\n";
    }
}

bool isSafe(int x, int y, const vector<vector<int>>& grid, vector<vector<bool>>& visited) {
    int rows = grid.size();
    int cols = grid[0].size();
    return (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 0 && !visited[x][y]);
}

void findShortestPath(int x, int y, const vector<vector<int>>& grid, vector<vector<bool>>& visited,
                      vector<pair<int, int>>& currentPath, vector<pair<int, int>>& shortestPath) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Base case: If the destination is reached, check if the current path is shorter
    if (x == rows - 1 && y == cols - 1) {
        if (currentPath.size() < shortestPath.size() || shortestPath.empty()) {
            shortestPath = currentPath;
        }
        return;
    }

    // Mark the current cell as visited
    visited[x][y] = true;

    // Explore all possible directions
    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;

        if (isSafe(newX, newY, grid, visited)) {
            currentPath.push_back({newX, newY});
            findShortestPath(newX, newY, grid, visited, currentPath, shortestPath);
            currentPath.pop_back();
        }
    }

    // Unmark the current cell (backtrack)
    visited[x][y] = false;
}

int main() {
    vector<vector<int>> grid = {
        {0, 0, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1},
        {1, 0, 0, 0}
    };

    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    vector<pair<int, int>> currentPath = {{0, 0}};
    vector<pair<int, int>> shortestPath;

    findShortestPath(0, 0, grid, visited, currentPath, shortestPath);

    if (!shortestPath.empty()) {
        cout << "Shortest path found: ";
        printPath(shortestPath);
        outputPathToCSV(shortestPath);
    } else {
        cout << "No path found to the destination." << endl;
    }

    return 0;
}
