#include "CNF_Formula.h"
#include <fstream>
#include <string>

/***** CONSTRUCTORS *****/

CNF_Formula::CNF_Formula(std::unordered_set<CNF_Clause*>* clauses) {
	this->clauses = clauses;
}

CNF_Formula::~CNF_Formula(void) {
	for (CNF_Clause* clause : *clauses) {
		delete clause;
	}

	delete clauses;
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

int CNF_Formula::generateDimacs(CNF_Formula* formula, std::string pathname) {
	std::ofstream file;
	file.open(pathname);
	if (file.fail()) {
		return -1;
	}

	for (CNF_Clause* clause : *formula->clauses) {
		file << clause->toString() << "0" << std::endl;
		if (file.fail()) {
			file.close();
			return -2;
		}
	}

	file.close();
	return 0;
}

int CNF_Formula::parseDimacs(CNF_Formula** formula, std::string pathname) {
	std::ifstream file;
	*formula = NULL;
	file.open(pathname);
	if (file.fail()) {
		return -1;
	}

	std::unordered_set<CNF_Clause*>* clauses = new std::unordered_set<CNF_Clause*>();
	std::string line;
	while (std::getline(file, line)) {
		CNF_Clause* clause = NULL;
		if (CNF_Clause::parseDimacs(&clause, line)) {
			file.close();
			delete clauses;
			return -2;
		}
		clauses->insert(clause);
	}

	file.close();

	*formula = new CNF_Formula(clauses);
	return 0;
}