#pragma once
#include <string.h>
#include <unordered_set>
#include "CNF_Clause.h"

class CNF_Clause;
class NNF_Formula;
class DPLL;
class Vertex_Cover;

class CNF_Formula {
	private:
		std::unordered_set<CNF_Clause*>* clauses;
		CNF_Formula(std::unordered_set<CNF_Clause*>*);

		friend class NNF_Formula;
		friend class DPLL;
		friend class Vertex_Cover;

	public:
		~CNF_Formula(void);

		std::string toString(void);

		/**
		 * Generate the dimacs file corresponding to the given formula
		 */
		static int generateDimacs(CNF_Formula*, std::string);

		/**
		 * Generate a CNF formula from a dimacs file
		 */
		static int parseDimacs(CNF_Formula**, std::string);

		/**
		 * Join multiple CNF formulae into one 
		 */
		static int combine(CNF_Formula**, CNF_Formula**, unsigned int len);
};
