#include <errno.h>
#include <iostream>
#include "CNF_Formula.h"
#include "NNF_Formula.h"

int main(const int argc, const char** argv) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <nnf_input_file> <cnf_output_file>" << std::endl;
		return 1;
	}

	NNF_Formula* formulaNNF;
	if (NNF_Formula::parseFile(&formulaNNF, argv[1])) {
		switch (errno) {
			case -1:
				std::cerr << "Error while opening file: " << argv[1] << std::endl;
				return errno;
			default:
				return errno;
		}
	}

	CNF_Formula* formulaCNF;
	if (NNF_Formula::tseitinTransform(&formulaCNF, formulaNNF)) {
		std::cerr << "Error while performing Tseitin's Transformation" << std::endl;
		return errno;
	}

	if (CNF_Formula::generateDimacs(formulaCNF, argv[2])) {
		switch (errno) {
			case -1:
				std::cerr << "Error while opening file: " << argv[2] << std::endl;
				return errno;
			case -2:
				std::cerr << "Error while writing to file" << argv[2] << std::endl;
				return errno;
			default:
				return errno;
		}
	}

	delete formulaNNF;
	delete formulaCNF;
	return 0;
}