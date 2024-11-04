// Project Back-end for autonomous robot navigation
// JanVas
// Iteration 3 & 4, backend code
//This code just covers the simple backtracking algorithm

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

//Simple backtracking algorithm 
extern "C" {
    bool findPath(vector<vector<int> > &maze, int x, int y, vector<vector<int> > &finalPath){
    int rows = maze.size();
    int columns = maze[0].size();
    // cout << "Rows is given as: " << rows << endl;
    if (x == rows-1 && y == columns-1 && maze[x][y] == 1){//if robot reaches destination
        finalPath[x][y] = 1;
        return true;
    }

    if(x>=0 && x<rows && y>=0 && y<columns && maze[x][y]==1){
        finalPath[x][y] = 1;
        maze[x][y] = 0; //Temporarily mark the cell as 0 to prevent infinite loops

        // Move right in the maze
        if(findPath(maze, x, y+1, finalPath))return true;
        //Move left in the maze
        if(findPath(maze, x, y-1, finalPath))return true;
        //Move up in the maze
        if(findPath(maze, x-1, y, finalPath))return true;
        //Move down in the maze
        if(findPath(maze, x+1, y, finalPath))return true;

        //Backtrack if neither true
        finalPath[x][y] = 0;
        maze[x][y] = 1;
        return false;
    }
    return false;
}

void printPath(vector<vector<int> > &finalPath){
    //Nested for loop to print the vectors that hold true or false for path followed
    for(int i=0; i<finalPath.size(); ++i){
        for(int j=0; j<finalPath[i].size(); ++j){
            if(finalPath[i][j] == 1){
                cout << "1 ";
            }
            if(finalPath[i][j] == 0){
                cout << "0 ";
            }
            // cout << (finalPath[i][j] ? "1 " : "0 ");
        }
        cout << endl;
    }
}
}

int main(){
    // Robot Navigation with Backtracking
    vector<vector<int> > maze ={{1, 0, 1, 1, 1},
                                {1, 1, 1, 0, 1},
                                {0, 1, 0, 1, 1},
                                {1, 0, 1, 0, 1}};

    vector<vector<int> > path(maze.size(), vector<int>(maze[0].size(), 0)); //intiialize path to be same size and filled with 0's
    
    if(findPath(maze, 0, 0, path)){
        printPath(path);
    } else {
        cout << "No path found!" << endl;
    }
    return 0;
}


