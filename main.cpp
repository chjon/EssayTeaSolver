#include <iostream>
#include "CNF_Formula.h"

int main(const int argc, const char** argv) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
		return 1;
	}

	CNF_Formula* formula;
	int error = CNF_Formula::parseDimacs(&formula, argv[1]);
	if (error == -1) {
		std::cerr << "Error while opening file" << std::endl;
		return error;
	} else if (error == -2) {
		std::cerr << "Error while parsing input" << std::endl;
		return error;
	}

	std::cout << formula->toString() << std::endl;

	error = CNF_Formula::generateDimacs(formula, argv[2]);
	if (error == -1) {
		std::cerr << "Error while opening file" << std::endl;
		return error;
	} else if (error == -2) {
		std::cerr << "Error while writing to file" << std::endl;
		return error;
	}

	return 0;
}