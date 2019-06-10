#pragma once
#include <unordered_set>
#include <string.h>

class CNF_Formula;
class NNF_Formula;

class CNF_Clause {
	private:
		std::unordered_set<int>* vars;

		CNF_Clause(std::unordered_set<int>*);
		~CNF_Clause(void);
		
		friend class CNF_Formula;
		friend class NNF_Formula;

	public:
		unsigned int size(void);
		bool isUnit(void);
		void bcp(int);
		std::string toString(void);
		static int parseDimacs(CNF_Clause**, std::string);
};