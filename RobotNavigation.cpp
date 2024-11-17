//Integrated Version 3 of code
//This uses backtacking as well as greedy
//It finds all possible routes before picking the one with the lowest weight
//Or it prioritizes lowest weights in a greedy method

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
using namespace std;

struct path{ //path used for the dynamic approach of finding all paths
    int totalWieght;//Total weight of the path taken, can be cost or time
    vector<vector<int>> pathTaken;//Vectors to store the path taken for this route
};

struct Node{ //
    int x, y;
    int weight;
    bool operator>(const Node& other) const{return weight > other.weight;}
};

//Method for checking if the x and y location are valid (wihtin bounds)
bool isValid(int x, int y, vector<vector<int>>& maze){
    int rows = maze.size();
    int cols = maze[0].size();
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] > 0);
}

//
void findAllPaths(vector<vector<int>>& maze, int x, int y, vector<vector<int>>& currentPath, 
    vector<path>& allPaths, int curWeight, int endX, int endY){  
    int rows = maze.size();
    int columns = maze[0].size();

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

//Greeedy (prim's MST-inspired) path
int primsShortestPath(vector<vector<int>>& maze, int startX, int startY, int endX, int endY, vector<vector<int>>& greedyPath){
    int rows = maze.size();
    int cols = maze[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));//track the parent cells
    greedyPath.assign(rows, vector<int>(cols, 0));//intiializes paths to all 0s

    //Uses a min-heap to prioritize the next cell with the lowest weight (greedy)
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({startX, startY, maze[startX][startY]});
    visited[startX][startY] = true;

    //Vectors for moving up, down, right and left
    vector<pair<int, int>> directions = {{1,0}, {0,1}, {-1, 0}, {0,-1}};

    while(!pq.empty()){
        Node current = pq.top();
        pq.pop();

        //If target reached the make the path and exit
        if(current.x == endX && current.y == endY){
            //Use parent array to trace back the path traveled
            int x_pos = endX;
            int y_pos = endY;
            while(x_pos != -1 && y_pos != -1){
                greedyPath[x_pos][y_pos] = 1;
                tie(x_pos, y_pos) = parent[x_pos][y_pos];
            }
            return current.weight;
        }

        //Explore neighboring cells and push into priority queue
        for(const auto& [x_move, y_move] : directions){
            int newX = current.x + x_move;
            int newY = current.y + y_move;

            if(isValid(newX, newY, maze) && !visited[newX][newY]){
                visited[newX][newY] = true;
                parent[newX][newY] = {current.x, current.y};
                pq.push({newX, newY, current.weight + maze[newX][newY]});
            }
        }
    }
    return -1; //return -1 if no path is found :(
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
        {1, 0, 2, 3, 1, 3, 5, 2, 2, 1, 4, 6, 1 ,3},
        {2, 3, 1, 0, 2, 8, 2, 5, 3, 2, 3, 2, 0, 2},
        {0, 2, 6, 4, 3, 4, 3, 2, 5, 1, 3, 2, 3, 0},
        {1, 7, 2, 0, 1, 6, 4, 2, 1, 3, 6, 2, 5, 3},
        {1, 0, 2, 3, 1, 0, 0, 2, 3, 2, 3, 1, 3, 5},
        {2, 3, 1, 0, 6, 1, 7, 2, 6, 1, 4, 5, 2, 2},
        {0, 2, 6, 4, 3, 0, 2, 2, 4, 5, 4, 2, 3, 1},
        {1, 7, 2, 0, 1, 1, 5, 2, 1, 0, 0, 0, 2, 1},
        {2, 3, 1, 0, 2, 8, 2, 5, 3, 2, 0, 1, 1, 3},
        {0, 2, 6, 4, 3, 4, 3, 2, 5, 1, 2, 3, 4, 5},
        {1, 7, 2, 0, 1, 6, 4, 2, 1, 3, 4, 2, 1, 7},
        {1, 0, 2, 3, 1, 0, 0, 2, 3, 2, 4, 2, 3, 2},
        {2, 3, 1, 0, 2, 1, 7, 2, 6, 1, 3, 3, 2, 1},
        {0, 2, 6, 4, 3, 0, 2, 2, 4, 5, 0, 0, 2, 1}
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
    int startX, startY, endX, endY, userSelect;
    cout << "Please enter start coordinates (x y): ";
    cin >> startX >> startY;
    cout << "Please enter end coordinates (x y): ";
    cin >> endX >> endY;
    if(startX < 0 || startX >= rows || startY < 0 || startY >= columns){
        cout << "Invalid start point based on boundaries! Please pick between:";
        cout << rows-1 << ", " << columns-1;        
        return -1;
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

    cout << "Please enter 1 for dynamic approach and 2 for greedy approach: ";
    cin >> userSelect;
    if(userSelect == 1){
        vector<path> allPaths;//store all possible paths (dynamic)
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
            pathToCSV(minPath.pathTaken, "output.csv");
        }
    }
    else if(userSelect == 2){
        vector<vector<int>> greedyPath;
        int minWeight = primsShortestPath(maze, startX, startY, endX, endY, greedyPath);

        if(minWeight == -1){
            cout << "No possible path found between start and end!!" << endl;
            return -1;
        } 
        else{
            cout << "Minimum path weight: " << minWeight << endl;
            cout << "Path traveled was: \n";
            printPath(greedyPath);
            pathToCSV(greedyPath, "output.csv");
        }
    }
    else{
        "Not a valid input for dynamic or greedy approach :(";
    }
    return 0;
}
