#include <iostream>
#include <bits/stdc++.h>
#define el '\n'
#include "Graph.h"
#include "Node.h"
using namespace std;

map<string, Graph> graphArray;

void Load() {
    //create an input file stream
    fstream file;
    file.open("Storage.txt", ios::in);
    if (!file.is_open())return;
    int graphCount;
    file >> graphCount;

    while (graphCount--) {
        string graphKey;
        file >> graphKey;

        Graph tempGraph;
        file >> tempGraph.isWeighted >> tempGraph.isDirected;

        int nodeCount;
        file >> nodeCount;

        while (nodeCount--) {
            string nodeKey;
            file >> nodeKey;

            Node tempNode;

            int adjSize;
            file >> adjSize;

            while (adjSize--) {
                string neighbour;
                int weight;
                file >> neighbour >> weight;
                tempNode.adj.push_back({ neighbour, weight });
            }

            tempGraph.nodeArray[nodeKey] = tempNode;
        }

        graphArray[graphKey] = tempGraph;
    }

    file.close();
}

void Save() {
    // create a file output stream
    ofstream file;
    file.open("Storage.txt");
    map<string, Graph>::iterator graph;
    map<string, Node>::iterator node;
    vector<pair<string, int>>::iterator adj;

    if (file.is_open()) {
        file << graphArray.size() << el;
        for (graph = graphArray.begin(); graph != graphArray.end(); graph++) {
            // displays the graph name and whether it's weighted or not and whether it's directed or not
            file << graph->first << ' ' << graph->second.isWeighted << ' ' << graph->second.isDirected << el;
            file << graphArray[(*graph).first].nodeArray.size() << el;

            for (node = graphArray[graph->first].nodeArray.begin(); node != graphArray[graph->first].nodeArray.end(); node++) {
                // displays the city name
                file << node->first << el;
                file << graphArray[graph->first].nodeArray[node->first].adj.size() << el;

                for (adj = graphArray[graph->first].nodeArray[node->first].adj.begin(); adj != graphArray[graph->first].nodeArray[node->first].adj.end(); adj++) {
                    // displays the city name and the weight/cost of its edge
                    file << adj->first << ' ' << adj->second << el;
                }

            }

        }
    }
    file.close();
}

void AddCity(string graphName) {
    cout << "Enter city name:\n>";
    string cityName; cin >> cityName;
    if (graphArray[graphName].nodeArray.find(cityName) == graphArray[graphName].nodeArray.end()) {
        graphArray[graphName].nodeArray[cityName];
    }
    else {
        cout << "Sorry, this city already exists." << el;
        return;
    }
}

void AddEdge(string graphName) {
    map<string, Node>& nodeArray = graphArray[graphName].nodeArray;

    string firstCity, secondCity;
    int cost = -1;

    cout << "Enter the name of the two cities you want to make a road between them:\n>";
    cin >> firstCity >> secondCity;

    if (nodeArray.find(firstCity) == nodeArray.end() || nodeArray.find(secondCity) == nodeArray.end()) {
        cout << "Invalid cities." << el;
        return;
    }
    // get the cost between them if it is weighted graph
    if (graphArray[graphName].isWeighted) {
        cout << "Enter the distance between them:\n>";
        cin >> cost;
    }

    graphArray[graphName].nodeArray[firstCity].adj.push_back({ secondCity,max(cost , 1) });

    // make a return edge between cities if it is not directed
    if (!graphArray[graphName].isDirected) {
        graphArray[graphName].nodeArray[secondCity].adj.push_back({ firstCity,max(cost, 1) });
    }

}

void AddGraph() {
    cout << "Enter graph name:\n>";
    string graphName; cin >> graphName;
    if (graphArray.find(graphName) != graphArray.end()) {
        cout << "Sorry, this graph already exists." << el;
        return;
    }
    cout << "Enter:\n[0] for undirected graph \n[1] for directed graph\n>";
    int isDirected;
    cin >> isDirected;
    if (isDirected != 0 && isDirected != 1) {
        cout << "You entered an invalid option.\n";
        return;
    }
    cout << "Enter:\n[0] for unweighted graph(BFS will be used)\n[1] for weighted graph(Dijkstra or Bellman Ford will be used)\n>";
    int isWeighted;
    cin >> isWeighted;
    if (isWeighted != 0 && isWeighted != 1) {
        cout << "You entered an invalid option.\n";
        return;
    }
    cout << "Enter number of cities you want in this graph:\n>";
    int numOfCities;
    cin >> numOfCities;
    if (numOfCities < 0) {
        cout << "You entered an invalid number of cities to add." << el;
        return;
    }
    graphArray[graphName].isDirected = isDirected;
    graphArray[graphName].isWeighted = isWeighted;
    while (numOfCities--) {
        AddCity(graphName);
    }
    cout << "Enter number of edges between the cities:\n>";
    int edgesCount;
    cin >> edgesCount;
    if (edgesCount < 0) {
        cout << "You entered an invalid number of edges to add." << el;
        return;
    }
    while (edgesCount--) {
        AddEdge(graphName);
    }

}

void DisplayGraph() {
    cout << "Enter the graph you want to display:\n>";
    string graphName; cin >> graphName;

    if (graphArray.find(graphName) == graphArray.end()) {
        cout << "Sorry, this graph does not exist." << el;
        return;
    }

    for (auto city : graphArray[graphName].nodeArray) {
        cout << city.first << ":";

        for (auto childCity : city.second.adj) {
            cout << '{' << childCity.first << ',' << childCity.second << '}' << ',';
        }
        cout << el;
    }
}

void DeleteCity(string graphName) {
    cout << "Please enter the name of the city you wish to delete:\n>";
    string cityName;
    cin >> cityName;
    map<string, Node>& currNodes = graphArray[graphName].nodeArray;

    // check if city exists
    if (currNodes.find(cityName) == currNodes.end()) {
        cout << "Sorry, this city does not exist." << el;
        return;
    }

    // remove incoming edges
    for (auto& node : currNodes) {
        vector<pair<string, int>>::iterator it;
        vector<pair<string, int>>& adj = node.second.adj;
        for (it = adj.begin(); it != adj.end(); it++) {
            if (it->first == cityName) {
                adj.erase(it);
                break;
            }
        }
    }
    // Delete node from nodeArray of graph
    currNodes.erase(cityName);
    cout << "City deleted successfully." << el;
}

void DeleteEdge(string graphName) {
    string cityOne, cityTwo;
    int weight = 1;
    map<string, Node>& currNodes = graphArray[graphName].nodeArray;
    cout << "Please enter the names of the two cities connected by the road:\n>";
    cin >> cityOne >> cityTwo;
    if (currNodes.find(cityOne) == currNodes.end() || currNodes.find(cityTwo) == currNodes.end()) {
        cout << "Sorry, the entered cities do not exist." << el;
        return;
    }

    if (graphArray[graphName].isWeighted) {
        cout << "Please enter the road's length:\n>";
        cin >> weight;
    }

    bool roadFound = false;
    vector<pair<string, int>> ::iterator it;
    vector<pair<string, int>>& cityOneAdj = currNodes[cityOne].adj;

    for (it = cityOneAdj.begin(); it != cityOneAdj.end(); it++) {
        string tempName = it->first;
        int tempWeight = it->second;

        if (tempName == cityTwo && tempWeight == weight) {
            roadFound = true;
            cityOneAdj.erase(it);
            break;
        }
    }

    vector<pair<string, int>>& cityTwoAdj = currNodes[cityTwo].adj;
    for (it = cityTwoAdj.begin(); it != cityTwoAdj.end(); it++) {
        string tempName = it->first;
        int tempWeight = it->second;

        if (tempName == cityOne && tempWeight == weight) {
            roadFound = true;
            cityTwoAdj.erase(it);
            break;
        }
    }

    if (roadFound) cout << "Road deleted successfully." << el;
    else cout << "Sorry, road does not exist." << el;
}

void UpdateGraph() {
    cout << "Enter graph name you want to update:\n>";
    string graphName;
    cin >> graphName;
    if (graphArray.find(graphName) == graphArray.end()) {
        cout << "Sorry, the entered graph does not exist." << el;
        return;
    }

    while (true) {
        cout << "Enter:\n[1] to add a new city \n[2] to add a new road\n[3] to delete city\n[4] to delete a road\n[5] to stop updating\n>";
        int op;
        cin >> op;
        if (op == 1) {
            AddCity(graphName);
            cout << "----------------------------------------" << el;
        }
        else if (op == 2) {
            AddEdge(graphName);
            cout << "----------------------------------------" << el;
        }
        else if (op == 3) {
            DeleteCity(graphName);
            cout << "----------------------------------------" << el;
        }
        else if (op == 4) {
            DeleteEdge(graphName);
            cout << "----------------------------------------" << el;
        }
        else if (op == 5) {
            break;
        }
        else cout << "Please enter a valid operation!\n";
    }

}

map<string, bool> visitedCity;

void DFS(string graphName, string cityName) {
    visitedCity[cityName] = 1;
    cout << cityName << el;

    for (auto nextCity : graphArray[graphName].nodeArray[cityName].adj) {

        if (!visitedCity[nextCity.first]) {
            if (graphArray[graphName].isWeighted)
                cout << nextCity.second << el; // cost between those cities

            DFS(graphName, nextCity.first);
        }
    }
}

void TraverseGraph() {
    cout << "Enter the graph name you want to traverse:\n>";
    string graphName; cin >> graphName;

    if (graphArray.find(graphName) == graphArray.end()) {
        cout << "Sorry, this graph does not exist." << el;
        return;
    }

    for (auto cityName : graphArray[graphName].nodeArray) {
        if (!visitedCity[cityName.first]) {
            cout << el;
            DFS(graphName, cityName.first);
        }
    }
    visitedCity.clear();
}

void Dijkstra(string graphName, string source, string destination, vector<string>& path, int& totalDistance) {
    //each city's previous city, used also to know which city is visited
    map<string, string> prev;

    map<string, int> distance;
    //first -> -ve distance, second -> node
    priority_queue<pair<int, string>> pq;
    pq.push({ 0, source });
    distance[source] = 0;
    prev[source] = source;

    while (!pq.empty()) {
        string currCity = pq.top().second;
        int currCost = -pq.top().first;
        pq.pop();

        //check if you already have a better answer
        if (distance[currCity] < currCost)
            continue;

        vector<pair<string, int>>& adj = graphArray[graphName].nodeArray[currCity].adj;
        for (auto& road : adj) {
            if (prev.find(road.first) == prev.end() || distance[road.first] > currCost + road.second) {
                prev[road.first] = currCity;
                distance[road.first] = currCost + road.second;
                pq.push({ -distance[road.first], road.first });
            }
        }
    }

    //if no path exists
    if (prev.find(destination) == prev.end())
        return;

    //build path
    totalDistance = distance[destination];
    string currCity = destination;
    while (prev[currCity] != currCity) {
        path.push_back(currCity);
        currCity = prev[currCity];
    }
    path.push_back(currCity);
}

//vector containing path
void BFS(string graphName, string source, string destination, vector<string>& path) {
    //each city's previous city, used also to know which city is visited
    map<string, string> prev;
    prev[source] = source;
    //first -> city, second -> level
    queue<string> qu;
    qu.push(source);

    while (!qu.empty()) {
        string currCity = qu.front();
        vector<pair<string, int>>& adj = graphArray[graphName].nodeArray[currCity].adj;
        qu.pop();

        for (auto& road : adj) {
            if (prev.find(road.first) == prev.end()) {
                prev[road.first] = currCity;
                qu.push(road.first);
            }
        }
    }

    //if no path exists
    if (prev.find(destination) == prev.end())
        return;

    //build path
    string currCity = destination;
    while (prev[currCity] != currCity) {
        path.push_back(currCity);
        currCity = prev[currCity];
    }
    path.push_back(currCity);
}

void Bellman(string graphName, string source, string destination, vector<string>& path, int& totalDistance) {
    //each city's previous city, used also to know which city is visited
    map<string, string> prev;
    prev[source] = source;

    //min distance from source to city
    map<string, int> distance;
    distance[source] = 0;

    //group all edges in graph
    //{{start, end}, weight}
    vector<pair<pair<string, string>, int> > edges;
    for (auto& node : graphArray[graphName].nodeArray) {
        for (auto& edge : node.second.adj) {
            edges.push_back({ {node.first, edge.first}, edge.second });
        }
    }

    for (int i = 0; i < graphArray[graphName].nodeArray.size(); i++) {
        for (auto& edge : edges) {
            string start = edge.first.first;
            string end = edge.first.second;
            int cost = edge.second;

            if (distance.find(start) != distance.end()) {
                //if inf or better answer
                if ((distance.find(end) == distance.end()) || (distance[start] + cost < distance[end])) {
                    distance[end] = distance[start] + cost;
                    prev[end] = start;
                }
            }
        }
    }

    //if no path exists
    if (prev.find(destination) == prev.end())
        return;

    //build path
    totalDistance = distance[destination];
    string currCity = destination;
    while (prev[currCity] != currCity) {
        path.push_back(currCity);
        currCity = prev[currCity];
    }
    path.push_back(currCity);
}

void FindPath() {
    cout << "Enter the name of the graph where you wish to find the shortest path:\n>";
    string graphName;
    cin >> graphName;

    if (graphArray.find(graphName) == graphArray.end()) {
        cout << "Sorry, this graph does not exist." << el;
        return;
    }

    map<string, Node>& currNodes = graphArray[graphName].nodeArray;

    cout << "Enter the starting city's name:\n>";
    string source;
    cin >> source;

    cout << "Enter the destination city's name:\n>";
    string destination;
    cin >> destination;

    if (currNodes.find(source) == currNodes.end() || currNodes.find(destination) == currNodes.end()) {
        cout << "Sorry, invalid city names." << el;
        return;
    }

    vector<string> path;
    int totalDistance = -1;
    if (graphArray[graphName].isWeighted) {
        cout << "Enter:\n[1] to use Dijkstra's algorithm\n[2] to use Bellman Ford's algorithm\n>";
        int op;
        cin >> op;

        if (op == 1) {
            Dijkstra(graphName, source, destination, path, totalDistance);
        }
        else {
            Bellman(graphName, source, destination, path, totalDistance);
        }
    }
    else {
        BFS(graphName, source, destination, path);
    }

    reverse(path.begin(), path.end());

    if (path.empty()) {
        cout << "A path between " << source << " and " << destination << " does not exist." << el;
        return;
    }

    if (~totalDistance)
        cout << "The total distance of the shortest path = " << totalDistance << el;

    cout << "Order of the cities visited on the shortest path:" << el;
    for (auto& i : path) {
        cout << i << ' ';
    }

    cout << el;
}

int main()
{
    Load();
    cout << "*******************************************" << el;
    cout << "********** Welcome to our system **********" << el;
    cout << "*******************************************" << el << el;

    while (true) {
        int op;
        cout << "Enter:\n[1] to add a graph\n[2] to display the graph\n[3] to update the graph\n[4] to traverse the graph\n[5] to find the shortest path between a source and a destination \n[6] to save and exit\n>";
        cin >> op;

        if (op == 1) {
            AddGraph();
            cout << "----------------------------------------" << el;
        }
        else if (op == 2) {
            DisplayGraph();
            cout << "----------------------------------------" << el;
        }
        else if (op == 3) {
            UpdateGraph();
        }
        else if (op == 4) {
            TraverseGraph();
            cout << "----------------------------------------" << el;
        }
        else if (op == 5) {
            FindPath();
            cout << "----------------------------------------" << el;
        }
        else if (op == 6) {
            break;
        }
        else {
            cout << "Please enter a valid operation!" << el;
        }
    }
    Save();
}