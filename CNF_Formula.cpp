#include "CNF_Formula.h"
#include <fstream>
#include <string>

/***** CONSTRUCTORS *****/

CNF_Formula::CNF_Formula(std::unordered_set<CNF_Clause*>* clauses) {
	this->clauses = clauses;
}

CNF_Formula::~CNF_Formula(void) {

}

/***** MEMBER FUNCTIONS *****/

std::string CNF_Formula::toString(void) {
	std::string s = "";
	for (CNF_Clause* clause : *clauses) {
		s += clause->toString() + " ";
	}
	return s;
}

/***** UTILITY FUNCTIONS *****/

CNF_Formula* CNF_Formula::parseDimacs(const char* pathname) {
	std::ifstream file;
	file.open(pathname);

	std::unordered_set<CNF_Clause*>* clauses = new std::unordered_set<CNF_Clause*>();
	std::string line;
	while (std::getline(file, line)) {
		CNF_Clause* clause = CNF_Clause::parseDimacs(line);
		if (clause == NULL) {
			file.close();
			delete clauses;
			return NULL;
		}
		clauses->insert(clause);
	}

	file.close();

	CNF_Formula* formula = new CNF_Formula(clauses);
	return formula;
}