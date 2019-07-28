#pragma once
#include "CNF_Formula.h"
#include "Graph.h"
#include <unordered_set>

class Vertex_Cover {
	private:
		/**
         * Generate clauses to prevent selecting more than k vertices
         */
        static void generateExclusionClauses(
            std::unordered_set<CNF_Clause*>* clauses,
            std::unordered_set<int>* varsSoFar,
            unsigned int k,
            int index,
            unsigned int numVertices
        );

	public:
        /**
         * Get a k-vertex cover for a graph if one exists
         */
        static std::unordered_set<int>* getVertexCover(Graph*, unsigned int k);
};
