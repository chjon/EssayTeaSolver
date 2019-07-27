#include "DPLL.h"
#include <errno.h>
#include <queue>

int DPLL::bcp(
	std::unordered_set<CNF_Clause*>* clauses,
	std::unordered_set<int>* assignments,
	std::unordered_set<int>* newAssignments
) {
	std::queue<int> unitClauses;
	for (int newAssignment : *newAssignments) {
		unitClauses.push(newAssignment);
	}
	
	/* Find initial unit clauses */
	for (CNF_Clause* clause : *clauses) {
		if (clause->isUnit()) {
			unitClauses.push(clause->getVar());
		}
	}

	/* Perform BCP */
	int satisfied = 0;
	while (!unitClauses.empty()) {
		/* Get unit variable */
		int unitVar = unitClauses.front();
		unitClauses.pop();

		/* Check for contradictory assignments */
		if (assignments->find(-unitVar) != assignments->end()) {
			return -1;
		}

		/* Add unit variable to assignments */
		assignments->insert(unitVar);
		newAssignments->insert(unitVar);

		/* Propagate variable */
		satisfied = 1;
		for (CNF_Clause* clause : *clauses) {
			if (clause->status() != CNF_CLAUSE_SATISFIED) {
				satisfied = 0;
				clause->assign(unitVar);

				/* Add variable to queue if clause is unit */
				if (clause->isUnit()) {
					unitClauses.push(clause->getVar());
				}

				/* Check for contradictory assignments */
				if (clause->status() == CNF_CLAUSE_CONTRADICT) {
					return -1;
				}
			}
		}
	}

	return satisfied;
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
	std::unordered_set<int> generatedAssignments;
	if (newAssignment) {
		generatedAssignments.insert(newAssignment);
	}

	/* Perform BCP and check for reduction to true/false */
	switch (bcp(clauses, assignments, &generatedAssignments)) {
		case -1:
			undoAssignments(clauses, assignments, &generatedAssignments);
			return false;
		case 1:
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

	/* Check if every clause is true */
	if (!selectedVar) {
		return true;
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
	undoAssignments(clauses, assignments, &generatedAssignments);
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