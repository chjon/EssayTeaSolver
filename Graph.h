#pragma once
#include <string>
#include <unordered_set>
#include "CNF_Clause.h"

class Vertex_Cover;

class Graph {
private:
    unsigned int numVertices;
    unsigned int numEdges;
    char** adjacencyMatrix;

    Graph(unsigned int numVertices, unsigned int numEdges, char** adjacencyMatrix);
	~Graph(void);

    friend Vertex_Cover;
public:
    std::string toString(void);
    static int parseGraph(Graph**, std::string filename);
};