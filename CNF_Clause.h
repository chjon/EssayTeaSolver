#pragma once
#include <unordered_set>
#include <string.h>
#include "CNF_Formula.h"

class CNF_Clause {
	private:
		std::unordered_set<int>* vars;
		friend class CNF_Formula;

	public:
		CNF_Clause(std::unordered_set<int>*);
		~CNF_Clause(void);

		unsigned int size(void);
		bool isUnit(void);
		void bcp(int);
		std::string toString(void);
		static CNF_Clause* parseDimacs(std::string);
};