#include "Graph.h"
#include <errno.h>
#include <fstream>
#include <iostream>
#include <unordered_set>

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
