#include "Vertex_Cover.h"
#include "DPLL.h"

/**
 * Generate exclusion clauses of size k + 1
 */
void Vertex_Cover::generateExclusionClauses(
    std::unordered_set<CNF_Clause*>* clauses,
    std::unordered_set<int>* varsSoFar,
    unsigned int k,
    int index,
    unsigned int numVertices
) {
    if ((int) k < 0) {
        std::unordered_set<int>* vars = new std::unordered_set<int>();
        for (int var : *varsSoFar) {
            vars->insert(var);
        }
        clauses->insert(new CNF_Clause(vars));
        return;
    }

    while (index <= (int)(numVertices - k)) {
        varsSoFar->insert(-index);
        generateExclusionClauses(clauses, varsSoFar, k - 1, index + 1, numVertices);
        varsSoFar->erase(-index);
        ++index;
    }
}

std::unordered_set<int>* Vertex_Cover::getVertexCover(Graph* graph, unsigned int k) {
    if (graph == NULL) {
        errno = -1;
        return NULL;
    }

    /* Generate clauses for each edge */
    std::unordered_set<CNF_Clause*>* clauses = new std::unordered_set<CNF_Clause*>();
    std::unordered_set<int>* vars;
    for (int i = 0; (unsigned) i < graph->numVertices; ++i) {
        for (int j = i; (unsigned) j < graph->numVertices; ++j) {
            if (graph->adjacencyMatrix[i][j]) {
                vars = new std::unordered_set<int>();
                vars->insert(i + 1);
                vars->insert(j + 1);
                clauses->insert(new CNF_Clause(vars));
            }
        }
    }

    /* Generate clauses to prevent selecting more than k vertices */
    std::unordered_set<int> varsSoFar;
    generateExclusionClauses(clauses, &varsSoFar, k, 1, graph->numVertices);

    /* Create CNF formula */
    CNF_Formula* f = new CNF_Formula(clauses);
    std::unordered_set<int>* satAssignment = DPLL::dpll(f);
    delete f;
    return satAssignment;
}