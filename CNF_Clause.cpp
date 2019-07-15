#include "CNF_Clause.h"
#include <sstream>

/***** CONSTRUCTORS *****/

CNF_Clause::CNF_Clause(std::unordered_set<int>* vars) :
	vars{vars},
	falseVars{new std::unordered_set<int>()},
	trueVar{0}
{}

CNF_Clause::~CNF_Clause(void) {
	delete vars;
	delete falseVars;
}

/***** MEMBER FUNCTIONS *****/

unsigned int CNF_Clause::size(void) {
	return vars->size();
}

bool CNF_Clause::isUnit(void) {
	return size() == 1;
}

int CNF_Clause::status(void) {
	if (trueVar) {
		return CNF_CLAUSE_SATISFIED;
	} else if (vars->size() == 0) {
		return CNF_CLAUSE_CONTRADICT;
	} else {
		return CNF_CLAUSE_UNKNOWN;
	}
}

int CNF_Clause::getVar(void) {
	if (vars->size() == 0) {
		return 0;
	} else {
		return *vars->begin();
	}
}

std::string CNF_Clause::toString(void) {
	std::string s = "";
	if (trueVar) {
		s += std::to_string(trueVar) + " ";
		return s;
	}

	for (int var : *vars) {
		s += std::to_string(var) + " ";
	}
	return s;
}

int CNF_Clause::assign(int assignedVar) {
	/* Check if clause is already satisfied */
	if (trueVar) {
		return 1;
	}

	/* Check if clause is already satisfied */
	if (vars->find(assignedVar) != vars->end()) {
		trueVar = assignedVar;
	}

	/* Reduce clause size */
	if (vars->find(-assignedVar) != vars->end()) {
		falseVars->insert(-assignedVar);
		vars->erase(-assignedVar);
	}

	return 0;
}

int CNF_Clause::unassign(std::unordered_set<int>* assignments) {
	if (assignments == NULL) {
		return -1;
	}

	for (int var : *assignments) {
		if (trueVar == var || trueVar == -var) {
			trueVar = 0;
		}
		
		if (falseVars->find(var) != falseVars->end()) {
			vars->insert(var);
			falseVars->erase(var);
		}

		if (falseVars->find(-var) != falseVars->end()) {
			vars->insert(-var);
			falseVars->erase(-var);
		}
	}

	return 0;
}

/***** UTILITY FUNCTIONS *****/

int CNF_Clause::parseDimacs(CNF_Clause** clause, const std::string line) {
	std::unordered_set<int>* vars = new std::unordered_set<int>();
	std::istringstream stream(line);
	int var = 0;
	*clause = NULL;

	while (stream >> var) {
		if (var == 0) {
			*clause = new CNF_Clause(vars);
			return 0;
		}

		vars->insert(var);
	}

	delete vars;
	return -1;
}