#pragma once
#include <iostream>
#include <bits/stdc++.h>
#define el '\n'
#include "Node.h"
using namespace std;

class Graph
{
public:
    bool isWeighted = false;

    bool isDirected = false;

    //use city name as index to get node
    map<string, Node> nodeArray;
};