#pragma once
#include <string>
#include <unordered_set>
#include "CNF_Clause.h"

class Graph {
private:
    unsigned int numVertices;
    unsigned int numEdges;
    char** adjacencyMatrix;

    Graph(unsigned int numVertices, unsigned int numEdges, char** adjacencyMatrix);
	~Graph(void);

    static void generateExclusionClauses(
        std::unordered_set<CNF_Clause*>* clauses,
        std::unordered_set<int>* varsSoFar,
        unsigned int k,
        int index,
        unsigned int numVertices
    );
public:
    std::string toString(void);
    static int parseGraph(Graph**, std::string filename);
    static std::unordered_set<int>* getVertexCover(Graph*, unsigned int k);
};