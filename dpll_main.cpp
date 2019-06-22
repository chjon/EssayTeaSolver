#include <iostream>
#include "DPLL.h"

int main(const int argc, const char** argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <dimacs_input_file>" << std::endl;
		return 1;
	}

	CNF_Formula* formula;
	if (CNF_Formula::parseDimacs(&formula, argv[1])) {
		switch (errno) {
			case -1:
				std::cerr << "Error while opening file: " << argv[1] << std::endl;
				return errno;
			case -2:
				std::cerr << "Invalid data format" << std::endl;
				return errno;
			default:
				break;
		}
	}

	std::cout << formula->toString() << std::endl;
	std::cout << "------------" << std::endl;
	DPLL::dpll(formula);
	std::cout << formula->toString() << std::endl;
}