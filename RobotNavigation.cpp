//Final integrated version of code (v4)
//This uses backtacking as well as greedy
//It finds all possible routes before picking the one with the lowest weight
//Or it prioritizes lowest weights in a greedy method
//Both methods have charger capabilities but dynamic garuntees optimal solution whereas greedy can scale up better

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
using namespace std;

struct path{ //path used for the dynamic approach of finding all paths
    int totalWieght;//Total weight of the path taken, can be cost or time
    vector<vector<int>> pathTaken;//Vectors to store the path taken for this route
    int remainingFuel;
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
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] > 0 || maze[x][y] == -1);
}

//helper function to return if a neighboring node is a charger
bool hasAdjacentCharger(const vector<vector<int>>& maze, int x, int y, int rows, int cols){
    vector<pair<int, int>> directions = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    for(const auto& [dx, dy] : directions) {
        int checkX = x + dx;
        int checkY = y + dy;
        if(checkX >= 0 && checkX < rows && checkY >= 0 && checkY < cols && 
           maze[checkX][checkY] == -1) {
            return true;
        }
    }
    return false;
}

void findAllPaths(vector<vector<int>>& maze, int x, int y, vector<vector<int>>& currentPath, 
    vector<path>& allPaths, int curWeight, int endX, int endY, int remainingFuel, int maxFuel){  
    int rows = maze.size();
    int columns = maze[0].size();

    //Check for boundary or out of bounds
    if(x < 0 || x >= rows || y < 0 || y >= columns || 
       (maze[x][y] <= 0 && maze[x][y] != -1)) return;

    //Check if fuel becomes exhausted
    int cellCost = (maze[x][y] == -1) ? 1 : maze[x][y];
    if(remainingFuel < cellCost) return;

    //End destination reached
    if (x == endX && y == endY) {
        currentPath[x][y] = 1;
        
        //Remaining fuel for when end destination reached
        int finalFuel = (maze[x][y] == -1) ? maxFuel : (remainingFuel - cellCost);
    
        allPaths.push_back({curWeight + cellCost, currentPath, finalFuel});
        
        currentPath[x][y] = 0;
        return;
    }

    //check to prevent infinite loops
    if(currentPath[x][y] == 1) return;

    //Mark current cell as visited
    currentPath[x][y] = 1;
    int originalWeight = maze[x][y];

    //Update remaining fuel
    int updatedFuel = (maze[x][y] == -1) ? maxFuel : (remainingFuel - cellCost);

    //Move in four directions (and check boundaries)
    vector<pair<int, int>> directions = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    for(const auto& [dx, dy] : directions) {
        int newX = x + dx;
        int newY = y + dy;

        //Check if new position is valid based on boundaries
        if(newX >= 0 && newX < rows && newY >= 0 && newY < columns) {
            findAllPaths(maze, newX, newY, currentPath, allPaths, 
                         curWeight + cellCost, endX, endY, updatedFuel, maxFuel);
        }
    }

    //Backtrack step
    currentPath[x][y] = 0;
    maze[x][y] = originalWeight;
}

//Find path with minimum weight, if tied then higher remaining fuel
path findMinPath(const vector<path>& allPaths){
    if(allPaths.empty()){
        throw runtime_error("No paths found!!");
    }
    path minPath = allPaths[0];
    for(size_t i=0; i<allPaths.size(); i++){
        const path& curPath = allPaths[i];
        if (curPath.totalWieght < minPath.totalWieght || (curPath.totalWieght == minPath.totalWieght && curPath.remainingFuel > minPath.remainingFuel)){
            minPath = curPath;
        }
    }
    return minPath;
}

int findGreedyPath(vector<vector<int>>& maze, int startX, int startY, int endX, int endY, 
                      vector<vector<int>>& greedyPath, int& fuel, int maxFuel, bool& fuelExhaustion){
    int rows = maze.size();
    int cols = maze[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
    greedyPath.assign(rows, vector<int>(cols, 0));
    // Track which chargers were used for refueling
    vector<vector<bool>> usedChargers(rows, vector<bool>(cols, false));

    int remainingFuel = fuel;
    fuelExhaustion = false;

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    vector<pair<int, int>> directions = {{1,0}, {0,1}, {-1,0}, {0,-1}};

    int startWeight = (maze[startX][startY] == -1) ? 1 : maze[startX][startY];
    pq.push({startX, startY, startWeight});
    visited[startX][startY] = true;

    while(!pq.empty()){
        Node current = pq.top();
        pq.pop();

        if(current.x == endX && current.y == endY){
            remainingFuel = fuel;
            vector<pair<int, int>> path;
            int x_pos = endX;
            int y_pos = endY;
            
            while(x_pos != -1 && y_pos != -1){
                path.push_back({x_pos, y_pos});
                tie(x_pos, y_pos) = parent[x_pos][y_pos];
            }

            // cout << "Printing the - Path and fuel calculations:" << endl;
            for(int i = path.size() - 1; i >= 0; i--){
                x_pos = path[i].first;
                y_pos = path[i].second;
                
                // Mark chargers that are used
                if(maze[x_pos][y_pos] == -1) {
                    remainingFuel = maxFuel;
                    // cout << "Position (" << x_pos << "," << y_pos << ") RECHARGING at charger to: " << remainingFuel << endl;
                    usedChargers[x_pos][y_pos] = true;
                } else if(hasAdjacentCharger(maze, x_pos, y_pos, rows, cols)){
                    remainingFuel = maxFuel;
                    // Find and mark the adjacent charger that was used
                    vector<pair<int, int>> adjacentDirs = {{1,0}, {0,1}, {-1,0}, {0,-1}};
                    for(const auto& [dx, dy] : adjacentDirs) {
                        int checkX = x_pos + dx;
                        int checkY = y_pos + dy;
                        if(checkX >= 0 && checkX < rows && checkY >= 0 && checkY < cols && 
                           maze[checkX][checkY] == -1) {
                            usedChargers[checkX][checkY] = true;
                            break;
                        }
                    }
                    // cout << "Position (" << x_pos << "," << y_pos << ") RECHARGING from adjacent charger to: " << remainingFuel << endl;
                }

                int cellCost = (maze[x_pos][y_pos] == -1) ? 1 : maze[x_pos][y_pos];
                
                // cout << "Position (" << x_pos << "," << y_pos << ") Cost: " << cellCost 
                    //  << " Fuel before: " << remainingFuel;

                if(remainingFuel < cellCost){
                    fuelExhaustion = true;
                    cout << endl;
                    return -1;
                }

                if(maze[x_pos][y_pos] == -1){
                    greedyPath[x_pos][y_pos] = -2;  // Charger on path
                } else {
                    greedyPath[x_pos][y_pos] = 1;  // Mark path
                    remainingFuel -= cellCost;
                }
                // cout << " Fuel after: " << remainingFuel << endl;
            }
            
            // After processing the path, mark used chargers that weren't on the path
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    if(maze[i][j] == -1 && usedChargers[i][j] && greedyPath[i][j] == 0){
                        greedyPath[i][j] = -2;  // Mark used chargers with 'C'
                    }
                }
            }
            
            fuel = remainingFuel;
            return current.weight;
        }

        for(const auto& [x_move, y_move] : directions){
            int newX = current.x + x_move;
            int newY = current.y + y_move;

            if(newX >= 0 && newX < rows && newY >= 0 && newY < cols && 
               (maze[newX][newY] > 0 || maze[newX][newY] == -1) && 
               !visited[newX][newY]) {
                
                int newWeight = (maze[newX][newY] == -1) ? 1 : maze[newX][newY];
                visited[newX][newY] = true;
                parent[newX][newY] = {current.x, current.y};
                pq.push({newX, newY, current.weight + newWeight});
            }
        }
    }
    return -1;
}


//Print the path out to terminal
void printPath(vector<vector<int>>& finalPath, vector<vector<int>>& maze){
    for(size_t i=0; i<finalPath.size(); i++){
        for(size_t j=0; j<finalPath[i].size(); j++){
            if(finalPath[i][j] == -2 || (finalPath[i][j] == 1 && maze[i][j] == -1)){
                cout << "C "; // Charger (either on path or used adjacent)
            }
            else if(finalPath[i][j] > 0){
                cout << "1 "; // Normal Path location
            }
            else if(finalPath[i][j] == -1 || maze[i][j] == -1){
                cout << "c "; // Unused charger
            }
            else{
                cout << "0 ";
            }
        }
        cout << endl;
    }
}

void pathToCSV(vector<vector<int>> pathTaken, vector<vector<int>> maze, string filename){
    ofstream outFile(filename);
    if(!outFile.is_open()){
        cout << "\n ERROR: Not able to open file " << filename <<endl;
        return;
    }
    for(size_t i=0; i<pathTaken.size(); i++){
        for(size_t j=0; j<pathTaken[i].size(); j++){
            if(maze[i][j] == -1 && pathTaken[i][j]!=-2){
                outFile << "-1 ";
            }
            else{
                outFile << pathTaken[i][j];
            }
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
    // Maze to traverse, 0s are obstacles
    vector<vector<int>> maze = {
        {1, 0, 2,-1, 7, 6},
        {2, 3, 1, 0, 2, 8},
        {0, 2, 5, 4, 3, 4},
        {1, 7, 0, 0, 1, 6},
        {2,-1, 1, 0, 2, 1},
        {0, 2, 6, 4, 3, 0}
    };
    
    // vector<vector<int>> maze = {
    //     {1, 0, 2, 3, 1, 3, 2,-1, 2, 1, 4, 6, 1 ,3},
    //     {2, 3, 1, 0, 2, 8, 2, 5, 3, 2, 3, 2, 0, 2},
    //     {0, 2, 6, 4, 3, 4, 1, 2, 5,-1, 3, 2, 3, 0},
    //     {1, 7, 2, 0, 1, 6, 4, 2, 1, 3, 6, 2, 5, 3},
    //     {1, 0, 2, 3, 1, 0, 0, 2, 3, 2, 3, 1, 3, 5},
    //     {2, 3, 1, 0, 6,-1, 7, 2, 6, 1, 4, 5, 2, 2},
    //     {0, 2, 6, 4, 3, 0, 2, 2, 4, 5, 4, 2, 3, 1},
    //     {1, 7, 2, 0, 1, 1, 5, 2, 1, 0, 0, 0, 2, 1},
    //     {2, 3, 1, 0, 2, 8, 2, 5, 3, 2, 0, 1, 1, 3},
    //     {0, 2, 6, 4, 3, 4, 3, 2, 5, 1,-1, 3, 4, 5},
    //     {1, 7, 2,-1, 1, 6, 4, 2, 1, 3, 4, 2, 1, 7},
    //     {1,-1, 2, 0, 1, 0,-1, 2, 3, 2, 4, 2, 3, 2},
    //     {2, 3, 1, 0, 2, 1, 7, 2, 6, 1, 3, 3, 2, 1},
    //     {0, 2, 6, 4, 3, 0, 2, 2, 4, 5, 0, 0, 2, 1}
    // };

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
    int startX, startY, endX, endY, userSelect, fuel, maxFuel;
    cout << "Please enter start coordinates (x y): ";
    cin >> startX >> startY;
    cout << "Please enter end coordinates (x y): ";
    cin >> endX >> endY;
    cout << "Please enter starting fuel: ";
    cin >> fuel;
    cout << "Please enter max fuel capacity: ";
    cin >> maxFuel;

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

        findAllPaths(maze, startX, startY, curPath, allPaths, 0, endX, endY, fuel, maxFuel);

        if(allPaths.empty()){
            cout << "No path found!" << endl;
            return -1;
        }
        else{
            path minPath = findMinPath(allPaths);
            cout << "Minimum path weight: " << minPath.totalWieght << endl;
            cout << "Path grid given as: \n";
            printPath(minPath.pathTaken, maze);
            cout << "\nRemaining fuel: " << minPath.remainingFuel << endl;
            pathToCSV(minPath.pathTaken, maze, "output.csv");
            return 0;
        }
    }
    if(userSelect == 2){
        vector<vector<int>> greedyPath;
        bool fuelExhaustion = false;
        int minWeight = findGreedyPath(maze, startX, startY, endX, endY, greedyPath, fuel, maxFuel, fuelExhaustion);

        if(minWeight == -1){
            if(fuelExhaustion){
                cout << "Path couldn't be completed due to fuel exhaustion." << endl; 
            }else{
                cout << "No possible path found between start and end!!" << endl;
            }
            return -1;
        } 
        else{
            cout << "Minimum path weight: " << minWeight << endl;
            cout << "Path traveled was: \n";
            printPath(greedyPath, maze);
            cout << "Remaining fuel: " << fuel << endl;
            pathToCSV(greedyPath, maze, "output.csv");
        }
    }
    else{
        cout << "Not a valid input for dynamic or greedy approach :(";
    }
    return 0;
}
