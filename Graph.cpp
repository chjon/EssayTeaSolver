#include "Graph.h"
#include <errno.h>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "CNF_Formula.h"
#include "DPLL.h"

Graph::Graph (unsigned int numVertices, unsigned int numEdges, char** adjacencyMatrix):
    numVertices{numVertices},
    numEdges{numEdges},
    adjacencyMatrix{adjacencyMatrix}
{}

Graph::~Graph () {
    for (unsigned int i = 0; i < numVertices; ++i) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;
}

int Graph::parseGraph(Graph** graph, std::string pathname) {
    std::ifstream file;
	*graph = NULL;
	file.open(pathname);
	if (file.fail()) {
		errno = -1;
		return errno;
	}

    unsigned int numVertices = 0;
    unsigned int numEdges = 0;
    char** adjacencyMatrix = NULL;

    std::string line;
    if (std::getline(file, line)) {
        for (unsigned int i = 0; i < line.length() && (line[i] == '0' || line[i] == '1'); ++i) {
            ++numVertices;
        }

        adjacencyMatrix = new char*[numVertices];
        for (unsigned int i = 0; i < numVertices; ++i) {
            adjacencyMatrix[i] = new char[numVertices];
            for (unsigned int j = 0; j < numVertices; ++j) {
                adjacencyMatrix[i][j] = -1;
            }
        }
    } else {
        file.close();
        errno = -2;
        return errno;
    }

    unsigned lineNum = 0;
    do {
        if (line.length() < numVertices) {
            file.close();
            errno = -3;
            return errno;
        }

        for (unsigned int i = 0; i < numVertices; ++i) {
            if (line[i] != '0' && line[i] != '1') {
                file.close();
                errno = -4;
                std::cout << line[i] << std::endl;
                return errno;
            }

            int val = line[i] - '0';

            if (adjacencyMatrix[lineNum][i] == -1) {
                adjacencyMatrix[lineNum][i] = val;
                adjacencyMatrix[i][lineNum] = val;
                if (val) {
                    numEdges++;
                }
            } else if (val != adjacencyMatrix[lineNum][i]) {
                file.close();
                errno = -5;
                return errno;
            }
        }

        ++lineNum;
    } while (std::getline(file, line));
    file.close();

    if (lineNum < numVertices) {
        errno = -6;
        return errno;
    }

	*graph = new Graph(numVertices, numEdges, adjacencyMatrix);
    return 0;
}

std::string Graph::toString(void) {
    std::string s = "";
    for (unsigned int i = 0; i < numVertices; ++i) {
        for (unsigned int j = 0; j < numVertices; ++j) {
            s += (char)(adjacencyMatrix[i][j] + '0');
        }
        s += "\n";
    }
    return s;
}

/**
 * Generate exclusion clauses of size k + 1
 */
void Graph::generateExclusionClauses(
    std::unordered_set<CNF_Clause*>* clauses,
    std::unordered_set<int>* varsSoFar,
    unsigned int k,
    int index,
    unsigned int numVertices
) {
    if ((int) k < 0) {
        std::unordered_set<int>* vars = new std::unordered_set<int>();
        for (int var : *varsSoFar) {
            vars->insert(var);
        }
        clauses->insert(new CNF_Clause(vars));
        return;
    }

    while (index <= (int)(numVertices - k)) {
        varsSoFar->insert(-index);
        generateExclusionClauses(clauses, varsSoFar, k - 1, index + 1, numVertices);
        varsSoFar->erase(-index);
        ++index;
    }
}

std::unordered_set<int>* Graph::getVertexCover(Graph* graph, unsigned int k) {
    if (graph == NULL) {
        errno = -1;
        return NULL;
    }

    /* Generate clauses for each edge */
    std::unordered_set<CNF_Clause*>* clauses = new std::unordered_set<CNF_Clause*>();
    std::unordered_set<int>* vars;
    for (int i = 0; (unsigned) i < graph->numVertices; ++i) {
        for (int j = i; (unsigned) j < graph->numVertices; ++j) {
            if (graph->adjacencyMatrix[i][j]) {
                vars = new std::unordered_set<int>();
                vars->insert(i + 1);
                vars->insert(j + 1);
                clauses->insert(new CNF_Clause(vars));
            }
        }
    }

    /* Generate clauses to prevent selecting more than k vertices */
    std::unordered_set<int> varsSoFar;
    generateExclusionClauses(clauses, &varsSoFar, k, 1, graph->numVertices);

    /* Create CNF formula */
    CNF_Formula* f = new CNF_Formula(clauses);
    std::unordered_set<int>* satAssignment = DPLL::dpll(f);
    delete f;
    return satAssignment;
}