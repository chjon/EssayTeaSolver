#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "CNF_Formula.h"

int main(const int argc, const char** argv) {
	CNF_Formula* formula = CNF_Formula::parseDimacs("test.dimacs");
	if (formula == NULL) {
		printf("Bad input\n");
		return 1;
	}

	std::cout << formula->toString() << std::endl;

	return 0;
}