#pragma once
#include <string.h>
#include <unordered_set>
#include "CNF_Clause.h"
#define DIMACS_LINE_LENGTH (512)

class CNF_Clause;

class CNF_Formula {
	private:
		std::unordered_set<CNF_Clause*>* clauses;

	public:
		CNF_Formula(std::unordered_set<CNF_Clause*>*);
		~CNF_Formula(void);
		std::string toString(void);

		static CNF_Formula* parseDimacs(const char*);
};
