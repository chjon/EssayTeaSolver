#include <iostream>
#include "Vertex_Cover.h"

int main (const int argc, const char** argv) {
    if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <graph_input_file> <k>" << std::endl;
		return 1;
	}

    Graph* g = NULL;
    if (Graph::parseGraph(&g, argv[1])) {
        switch (errno) {
            case -1:
                std::cerr << "Could not open file" << std::endl;
                return errno;
            case -2:
                std::cerr << "Problem reading line" << std::endl;
                return errno;
            case -3:
                std::cerr << "Incorrect line length" << std::endl;
                return errno;
            case -4:
                std::cerr << "Received invalid token" << std::endl;
                return errno;
            case -5:
                std::cerr << "Edge value does not match previously read value" << std::endl;
                return errno;
            case -6:
                std::cerr << "Expecting more lines" << std::endl;
                return errno;
        }
    }

    std::unordered_set<int>* satAssignment = Vertex_Cover::getVertexCover(g, atoi(argv[2]));;
	if (satAssignment) {
		std::cout << "SAT: ";
		for (int var : *satAssignment) {
			std::cout << var << " ";
		}
		std::cout << std::endl;
		delete satAssignment;
	} else {
		std::cout << "UNSAT" << std::endl;
	}

    return 0;
}