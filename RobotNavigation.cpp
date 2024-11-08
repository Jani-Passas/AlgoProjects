//Integrated Version 1 of code
//This uses backtacking (not really greedy)
//It finds all possible routes 
//before picking the one with the lowest weight

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct path{
    int totalWieght;//Total weight of the path taken, can be cost or time
    vector<vector<int>> pathTaken;//Vectors to store the path taken for this route
};

//
void findAllPaths(vector<vector<int>>& maze, int x, int y, 
    vector<vector<int>>& currentPath, vector<path>& allPaths, int curWeight){
    int rows = maze.size();
    int columns = maze[0].size();

    //Check if destination reached
    if(x == rows-1 && y == columns-1 && maze[x][y] > 0){
        currentPath[x][y] = 1;
        allPaths.push_back({curWeight + maze[x][y], currentPath});
        currentPath[x][y] = 0;
        return;
    }

    //Boundary checking to prevent seg faults
    if(x < 0 || x >= rows || y < 0 || y >= columns || maze[x][y] <= 0) return;

    //Mark the current cell in the path to be taken
    currentPath[x][y] = 1;
    int weight = maze[x][y];
    maze[x][y] = -1; //mark this cell as visited by setting to -1

    //Go in all four directions to explore possible paths
    if(x+1 < rows){//Move down
        findAllPaths(maze, x+1, y, currentPath, allPaths, curWeight + weight);
    }
    if(y+1 < columns){//Move right
        findAllPaths(maze, x, y+1, currentPath, allPaths, curWeight + weight);
    }
    if(x-1 < rows){//Move up
        findAllPaths(maze, x-1, y, currentPath, allPaths, curWeight + weight);
    }
    if(y-1 < columns){//Move left
        findAllPaths(maze, x, y-1, currentPath, allPaths, curWeight + weight);
    }
    //Backtrack and reset to current weight
    currentPath[x][y] = 0;
    maze[x][y] = weight;
}

//Find path with minimum weight
path findMinPath(const vector<path>& allPaths){
    path minPath = allPaths[0];
    for(size_t i=0; i<allPaths.size(); i++){
        const path& curPath = allPaths[i];
        if(curPath.totalWieght < minPath.totalWieght){
            minPath = curPath;
        }
    }
    return minPath;
}

//Print the path out to terminal
void printPath(vector<vector<int>>& finalPath){
    for(size_t i=0; i<finalPath.size(); i++){
        for(size_t j=0; j<finalPath[0].size(); j++){
            if(finalPath[i][j]>0){
                cout << "1 "; //For now just printing 1s for the path taken
            }
            else if(finalPath[i][j] == 0){
                cout << "0 ";
            }
            else{
                cout << "THERE IS A NEGATIVE IN THE FINAL PATH?";
                return;
            }
        }
        cout << endl;
    }
}

int main(){
    //Maze to traverse, 0s are obstacles
    vector<vector<int>> maze = {
        {1, 0, 2, 3, 1},
        {2, 3, 1, 0, 2},
        {0, 2, 6, 4, 3},
        {1, 7, 2, 0, 1}
    };
    int rows = maze.size();
    int columns = maze[0].size();

    vector<path> allPaths;//store all possible paths
    vector<vector<int>> curPath(rows, vector<int>(columns,0));//creates a 2D array of all zeroes to track cur path

    findAllPaths(maze, 0, 0, curPath, allPaths, 0);

    if(allPaths.empty()){
        cout << "No path found!" << endl;
        return -1;
    }
    else{
        path minPath = findMinPath(allPaths);
        cout << "Minimum path weight: " << minPath.totalWieght << endl;
        cout << "Path grid given as: \n";
        printPath(minPath.pathTaken);
    }
    return 0;
}
