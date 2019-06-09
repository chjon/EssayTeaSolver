#pragma once
#include <string.h>
#include <unordered_set>
#include "CNF_Clause.h"

class CNF_Clause;

class CNF_Formula {
	private:
		std::unordered_set<CNF_Clause*>* clauses;

		CNF_Formula(std::unordered_set<CNF_Clause*>*);

	public:
		~CNF_Formula(void);

		std::string toString(void);

		static int generateDimacs(CNF_Formula*, std::string);
		static int parseDimacs(CNF_Formula**, std::string);
};
