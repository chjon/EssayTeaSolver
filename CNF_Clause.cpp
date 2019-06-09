#include "CNF_Clause.h"
#include <sstream>
#include <iostream>
#define ATOI_BUFFER_LENGTH (32)

/***** CONSTRUCTORS *****/

CNF_Clause::CNF_Clause(std::unordered_set<int>* vars) {
	this->vars = vars;
}

CNF_Clause::~CNF_Clause(void) {
	delete vars;
}

/***** MEMBER FUNCTIONS *****/

unsigned int CNF_Clause::size(void) {
	return vars->size();
}

bool CNF_Clause::isUnit(void) {
	return size() == 1;
}

void CNF_Clause::bcp(int) {

}

std::string CNF_Clause::toString(void) {
	std::string s = "";
	for (int var : *vars) {
		s += std::to_string(var) + " ";
	}
	return s;
}

/***** UTILITY FUNCTIONS *****/

CNF_Clause* CNF_Clause::parseDimacs(const std::string line) {
	std::unordered_set<int>* vars = new std::unordered_set<int>();
	std::istringstream stream(line);
	int var = 0;

	while (stream >> var) {
		if (var == 0) {
			return new CNF_Clause(vars);
		}

		vars->insert(var);
	}

	delete vars;
	return NULL;
}