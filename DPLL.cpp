#include "DPLL.h"
#include <queue>

std::unordered_set<int>* DPLL::bcp(
	std::unordered_set<CNF_Clause*>* clauses,
	std::unordered_set<int>* assignments,
	int newAssignment
) {
	std::unordered_set<int>* bcpAssignments = new std::unordered_set<int>();
	std::queue<int> unitClauses;
	if (newAssignment) {
		unitClauses.push(newAssignment);
	}
	
	/* Find initial unit clauses */
	for (CNF_Clause* clause : *clauses) {
		if (clause->isUnit()) {
			unitClauses.push(clause->getVar());
		}
	}

	/* Perform BCP */
	while (!unitClauses.empty()) {
		/* Get unit variable */
		int unitVar = unitClauses.front();
		unitClauses.pop();

		/* Check for contradictory assignments */
		if (assignments->find(-unitVar) != assignments->end()) {
			undoAssignments(clauses, assignments, bcpAssignments);
			delete bcpAssignments;
			return NULL;
		}

		/* Add unit variable to assignments */
		assignments->insert(unitVar);
		bcpAssignments->insert(unitVar);

		/* Propagate variable */
		for (CNF_Clause* clause : *clauses) {
			if (clause->status() == CNF_CLAUSE_SATISFIED) {
				continue;
			}

			clause->assign(unitVar);

			/* Add variable to queue if clause is unit */
			if (clause->isUnit()) {
				unitClauses.push(clause->getVar());
			}

			/* Check for contradictory assignments */
			if (clause->status() == CNF_CLAUSE_CONTRADICT) {
				undoAssignments(clauses, assignments, bcpAssignments);
				delete bcpAssignments;
				return NULL;
			}
		}
	}

	return bcpAssignments;
}

std::unordered_set<int>* DPLL::plp(
	std::unordered_set<CNF_Clause*>* clauses,
	std::unordered_set<int>* assignments
) {
	return 0;
}

void DPLL::undoAssignments(
	std::unordered_set<CNF_Clause*>* clauses,
	std::unordered_set<int>* assignments,
	std::unordered_set<int>* toUndo
) {
	/* Undo clause assignments */
	for (CNF_Clause* clause : *clauses) {
		clause->unassign(toUndo);
	}

	/* Undo variable assignments */
	for (int var : *toUndo) {
		assignments->erase(var);
	}
}

bool DPLL::dpllHelper(
	std::unordered_set<CNF_Clause*>* clauses,
	std::unordered_set<int>* assignments,
	int newAssignment
) {
	/* Perform BCP */
	std::unordered_set<int>* newAssignments = bcp(clauses, assignments, newAssignment);

	/* Check if the formula has been reduced to false */
	if (newAssignments == NULL) {
		return false;
	}

	/* Check if the formula has been reduced to true */
	bool satisfied = true;
	for (CNF_Clause* clause : *clauses) {
		if (clause->status() != CNF_CLAUSE_SATISFIED) {
			satisfied = false;
			break;
		}
	}

	if (satisfied) {
		return true;
	}

	/* Select an unassigned variable */
	int selectedVar = 0;
	for (CNF_Clause* clause : *clauses) {
		if (clause->status() == CNF_CLAUSE_SATISFIED) {
			continue;
		}

		for (int var : *clause->vars) {
			if (
				assignments->find( var) == assignments->end() &&
				assignments->find(-var) == assignments->end()
			) {
				selectedVar = var;
				break;
			}
		}

		if (selectedVar) {
			break;
		}
	}

	/* Try assigning the variable to true */
	if (dpllHelper(clauses, assignments, selectedVar)) {
		return true;
	}

	/* Try assigning the variable to false */
	if (dpllHelper(clauses, assignments, -selectedVar)) {
		return true;
	}

	/* Undo assignments */
	undoAssignments(clauses, assignments, newAssignments);

	delete newAssignments;
	return false;
}

std::unordered_set<int>* DPLL::dpll(CNF_Formula* formula) {
	std::unordered_set<int>* assignments = new std::unordered_set<int>();

	if (dpllHelper(formula->clauses, assignments, 0)) {
		return assignments;
	}

	delete assignments;
	return NULL;
}