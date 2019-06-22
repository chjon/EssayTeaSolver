#pragma once
#include <string.h>
#include <unordered_set>
#include "CNF_Clause.h"

class CNF_Clause;
class NNF_Formula;
class DPLL;

class CNF_Formula {
	private:
		std::unordered_set<CNF_Clause*>* clauses;
		CNF_Formula(std::unordered_set<CNF_Clause*>*);

		friend NNF_Formula;
		friend DPLL;

	public:
		~CNF_Formula(void);

		std::string toString(void);

		static int generateDimacs(CNF_Formula*, std::string);
		static int parseDimacs(CNF_Formula**, std::string);
		static int combine(CNF_Formula**, CNF_Formula**, unsigned int len);
};
