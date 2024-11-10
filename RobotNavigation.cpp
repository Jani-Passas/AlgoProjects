//Integrated Version 1 of code
//This uses backtacking (not really greedy)
//It finds all possible routes 
//before picking the one with the lowest weight

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct path{
    int totalWieght;//Total weight of the path taken, can be cost or time
    vector<vector<int>> pathTaken;//Vectors to store the path taken for this route
};

//
void findAllPaths(vector<vector<int>>& maze, int x, int y, vector<vector<int>>& currentPath, 
    vector<path>& allPaths, int curWeight, int endX, int endY){  
    int rows = maze.size();
    int columns = maze[0].size();

    //Check if destination reached
    // if(x == rows-1 && y == columns-1 && maze[x][y] > 0){
    //     currentPath[x][y] = 1;
    //     allPaths.push_back({curWeight + maze[x][y], currentPath});
    //     currentPath[x][y] = 0;
    //     return;
    // }

    if (x == endX && y == endY && maze[x][y] > 0) {
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
        findAllPaths(maze, x+1, y, currentPath, allPaths, curWeight + weight, endX, endY);
    }
    if(y+1 < columns){//Move right
        findAllPaths(maze, x, y+1, currentPath, allPaths, curWeight + weight, endX, endY);
    }
    if(x-1 < rows){//Move up
        findAllPaths(maze, x-1, y, currentPath, allPaths, curWeight + weight, endX, endY);
    }
    if(y-1 < columns){//Move left
        findAllPaths(maze, x, y-1, currentPath, allPaths, curWeight + weight, endX, endY);
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
        for(size_t j=0; j<finalPath[i].size(); j++){
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
void pathToCSV(vector<vector<int>> pathTaken, string filename){
    ofstream outFile(filename);
    if(!outFile.is_open()){
        cout << "\n ERROR: Not able to open file " << filename <<endl;
        return;
    }
    for(size_t i=0; i<pathTaken.size(); i++){
        for(size_t j=0; j<pathTaken[i].size(); j++){
            outFile << pathTaken[i][j];
            if(j<pathTaken[i].size()-1){
                outFile << ", ";
            }
        }
        outFile << "\n";
    }
    outFile.close();
    cout << "Data saved into CSV file and written to: " << filename << endl;
}

int main(){
    //Maze to traverse, 0s are obstacles
    vector<vector<int>> maze = {
        {1, 0, 2, 3, 1, 3, 5, 2, 2, 1},
        {2, 3, 1, 0, 2, 8, 2, 5, 3, 2},
        {0, 2, 6, 4, 3, 4, 3, 2, 5, 1},
        // {1, 7, 2, 0, 1, 6, 4, 2, 1, 3},
        // {1, 0, 2, 3, 1, 0, 0, 2, 3, 2},
        {2, 3, 1, 0, 2, 1, 7, 2, 6, 1},
        {0, 2, 6, 4, 3, 0, 2, 2, 4, 5},
        {1, 7, 2, 0, 1, 1, 5, 2, 1, 0}
    };

    for(size_t i=0; i<maze.size(); i++){
        for(size_t j=0; j<maze[i].size(); j++){
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    int rows = maze.size();
    int columns = maze[0].size();
    cout << "Boundaries for the input are " << rows-1 << " and " << columns-1 << endl;


    //Get user inputs
    int startX, startY, endX, endY;
    cout << "Please enter start coordinates (x y): ";
    cin >> startX >> startY;
    cout << "Please enter end coordinates (x y): ";
    cin >> endX >> endY;


    if(startX < 0 || startX >= rows || startY < 0 || startY >= columns){
        cout << "Invalid start point based on boundaries! Please pick between:";
        cout << rows-1 << ", " << columns-1;        return -1;
    }

    if(endX < 0 || endX >= rows || endY < 0 || endY >= columns){
        cout << "Invalid end point based on boundaries! Please pick between:";
        cout << rows-1 << ", " << columns-1;
        return -1;
    }

    if(maze[startX][startY] == 0 || maze[endX][endY] == 0){
        cout << "Invalid start or end point based on values!!";
        return -1;
    }

    vector<path> allPaths;//store all possible paths
    vector<vector<int>> curPath(rows, vector<int>(columns,0));//creates a 2D array of all zeroes to track cur path

    findAllPaths(maze, startX, startY, curPath, allPaths, 0, endX, endY);

    if(allPaths.empty()){
        cout << "No path found!" << endl;
        return -1;
    }
    else{
        path minPath = findMinPath(allPaths);
        cout << "Minimum path weight: " << minPath.totalWieght << endl;
        cout << "Path grid given as: \n";
        printPath(minPath.pathTaken);
        pathToCSV(minPath.pathTaken, "Test1.csv");
    }
    return 0;
}
