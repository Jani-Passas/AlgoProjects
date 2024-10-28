// Project Back-end for autonomous robot navigation
// JanVas
// Iteration 3 & 4, backend code
//This code just covers the ideas of MST prior to integration

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Edge{
    int u,v, weight;
};

//Printing function for the graph
void printMST(vector<Edge>& MST_edges){
    cout<< "The edges in MST are given as:\n";
    for(int i=0; i<MST_edges.size(); ++i){
        cout << MST_edges[i].u << " - " << MST_edges[i].v << " : " << MST_edges[i].weight << endl;
    }
}

//Prim's Algorithm to find MST for a graph, resources online
vector<Edge> MST(int vertex, vector<vector<pair<int, int>>>& graph){
    vector<Edge> MST_edges; //store edges of MST
    vector<int> MST_key(vertex, INT_MAX); //store min weight to include that vertex
    vector<bool> InMST(vertex, false); //Indicates if vertex is/isn't in MST
    vector<int> parent(vertex, -1); //stores the parent of the vertex in MST

    MST_key[0] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> priorityQueue;
    priorityQueue.push(make_pair(0,0)); //Weight, vertex pair

    while(!priorityQueue.empty()){
        int u = priorityQueue.top().second;
        priorityQueue.pop();
        InMST[u] = true; //include u in the MST

        //iterate through adjacent vertices
        for(int i=0; i<graph[u].size(); ++i){
            int v = graph[u][i].first;
            int weight = graph[u][i].second;

            if(!InMST[v] && MST_key[v] > weight){
                MST_key[v] = weight;
                priorityQueue.push(make_pair(MST_key[v], v));
                parent[v] = u;
            }
        }
    }
    //Store edges in MST based on parent
    for(int i=1; i<vertex; ++i){
        if(parent[i] != -1){
            MST_edges.push_back({parent[i], i, MST_key[i]});
        }
    }
    return MST_edges;
}


int main(){
    int vertex = 5;
    vector<vector<pair<int, int>>> graph(vertex);
    graph[0].push_back(make_pair(1, 2));
    graph[1].push_back(make_pair(0, 2));
    graph[1].push_back(make_pair(2, 3));
    graph[2].push_back(make_pair(1, 3));
    graph[2].push_back(make_pair(3, 1));
    graph[3].push_back(make_pair(2, 1));
    graph[3].push_back(make_pair(4, 6));
    graph[4].push_back(make_pair(3, 6));

    vector<Edge> MST_edges = MST(vertex, graph);
    printMST(MST_edges);
}
