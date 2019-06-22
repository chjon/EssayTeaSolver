#include "DPLL.h"
#include <queue>

std::unordered_set<int>* DPLL::bcp(std::vector<CNF_Clause*>* clauses, std::unordered_set<int>* assignments) {
	std::unordered_set<int>* bcpAssignments = new std::unordered_set<int>();
	std::queue<int> unitClauses;
	
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
			undoAssignments(clauses, bcpAssignments);
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
				undoAssignments(clauses, bcpAssignments);
				delete bcpAssignments;
				return NULL;
			}
		}
	}

	return bcpAssignments;
}

std::unordered_set<int>* DPLL::plp(std::vector<CNF_Clause*>* clauses, std::unordered_set<int>* assignments) {
	return 0;
}

void DPLL::undoAssignments(std::vector<CNF_Clause*>* clauses, std::unordered_set<int>* assignments) {
	for (CNF_Clause* clause : *clauses) {
		clause->unassign(assignments);
	}
}