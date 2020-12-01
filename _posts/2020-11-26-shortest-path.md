---
layout:     post
title:      shortest path
subtitle:   
date:       2020-11-26
author:     zhamao
timecost:   5 minutes
# header-style: text
# header-mask: 0.01
nav-style:      invert
header-img-credit:      Chen Yuhan
header-img-year:        2020
header-img-outchain:    false
header-img: img/post-bg-unix-linux.jpg
catalog: true
mathjax: false
live2d:  false
tags:
    - Matrix
    - Haiyan's New Trick
---


# Shortest Path (Dijkstra's Algorithm)

- 1000 ms / 32 MB
- By 乔海燕 (qiaohy@mail.sysu.edu.cn)

## Problem

Given a directed graph, compute all the shortest paths from the source to other vertices.

## Input

The first line is the number of test cases.

For every test case, the first line is the number of node n, meaning nodes are 1,2,…,n.

The next line is the number of edges m, then m lines are followed, where each line is in the form of u v w, meaning (u,v) is an edge and it has weight w (>0).

The last line is the source vertex.

## Output

For each test case, print all the shortest paths from the source s to other vertices (if the shortest path is finite) in the order of vertices in the following form （no spaces between）:

s-1:d1

s-2:d2

...

## Sample Input

```
2
2
1
1 2 5
1
5
6
1 2 4
1 3 6
2 3 1
4 5 1
3 1 1
5 4 2
2
```

## Sample Output

```
1-1:0
1-2:5
2-1:2
2-2:0
2-3:1
```

## Hint

Suggested solution: again, write a few functions.

- A function which constructs the graph:

```cpp
Graph mkGraph();
```

- A function which computes all the shortest paths, for example:

```cpp
vector<int> dijkstra(Graph g, Vertex source);
```

- A print function which prints the result as required:

```cpp
void print(vector<int> d, Vertex source);
```

## Answer

```cpp
// shortest_path.cpp
#include <iostream>
#include <map>
#include <vector>

#define INFINITY 2147483647

using namespace std;

typedef int Vertex;
typedef int Weight;
typedef vector<vector<Weight>> Graph;

// A function which constructs the graph
Graph mkGraph() {
    int nodeNum, edgeNum;
    cin >> nodeNum >> edgeNum;
    vector<Weight> initVec(nodeNum, INFINITY);
    Graph adjGraph(nodeNum, initVec);

    Vertex u, v;
    Weight w;
    for (size_t i = 0; i < edgeNum; i++) {
        cin >> u >> v >> w;
        adjGraph[u - 1][v - 1] = w;
    }
    return adjGraph;
}

// A function which computes all the shortest paths
vector<Weight> dijkstra(Graph g, Vertex source) {

    int n = g.size();

    vector<Weight> dist(n, INFINITY);
    vector<bool> known(n, false);

    for (size_t i = 0; i < n; i++)
        dist[i] = g[source - 1][i];

    dist[source - 1] = 0;
    known[source - 1] = true;

    for (size_t i = 0; i < n; i++) {
        Vertex v = -1;
        int m = INFINITY;

        for (size_t i = 0; i < n; i++) {
            if (!known[i] && dist[i] < m) {
                v = i;
                m = dist[i];
            }
        }

        if (v == -1) break;

        known[v] = true;
        for (size_t i = 0; i < n; i++) {
            if (!known[i] && (g[v][i] < INFINITY)) { // or dist + cvw will overflow
                if (dist[v] + g[v][i] < dist[i])
                    dist[i] = dist[v] + g[v][i];
            }
        }
    }
    return dist;
}

// A print function which prints the result as required
void print(vector<Weight> d, Vertex source) {
    for (size_t i = 0; i < d.size(); i++)
        if (d[i] < INFINITY)
            cout << source << "-" << (i + 1) << ":" << d[i] << endl;
}

void shortest_path() {
    int sourceVertex;
    Graph g = mkGraph();
    cin >> sourceVertex;
    vector<Weight> d = dijkstra(g, sourceVertex);
    print(d, sourceVertex);
}

int main() {
    int T;
    cin >> T;
    while (T--) shortest_path();
    return 0;
}
```
