#include <errno.h>
#include <iostream>
#include "CNF_Formula.h"
#include "NNF_Formula.h"

int main(const int argc, const char** argv) {
	if (argc == 2) {
		NNF_Formula* formula;
		if (NNF_Formula::parseFile(&formula, argv[1])) {
			std::cout << "Error while parsing file" << std::endl;
		} else {
			std::cout << formula->toString() << std::endl;
		}

		return errno;
	}

	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
		return 1;
	}

	CNF_Formula* formula;
	if (CNF_Formula::parseDimacs(&formula, argv[1])) {
		switch (errno) {
			case -1:
				std::cerr << "Error while opening file" << std::endl;
				return errno;
			case -2:
				std::cerr << "Error while parsing input" << std::endl;
				return errno;
			default:
				return errno;
		}
	}

	std::cout << formula->toString() << std::endl;

	if (CNF_Formula::generateDimacs(formula, argv[2])) {
		switch (errno) {
			case -1:
				std::cerr << "Error while opening file" << std::endl;
				return errno;
			case -2:
				std::cerr << "Error while writing to file" << std::endl;
				return errno;
			default:
				return errno;
		}
	}

	delete formula;
	return 0;
}