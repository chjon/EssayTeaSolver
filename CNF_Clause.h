#pragma once
#include <unordered_set>
#include <string.h>

#define CNF_CLAUSE_UNKNOWN (0)
#define CNF_CLAUSE_SATISFIED (1)
#define CNF_CLAUSE_CONTRADICT (2)

class CNF_Formula;
class NNF_Formula;
class DPLL;

class CNF_Clause {
	private:
		std::unordered_set<int>* vars;
		std::unordered_set<int>* falseVars;
		int trueVar;

		CNF_Clause(std::unordered_set<int>*);
		~CNF_Clause(void);
		
		friend class CNF_Formula;
		friend class NNF_Formula;
		friend class DPLL;

	public:
		/***** Accessors *****/
		unsigned int size(void);
		bool isUnit(void);
		int status(void);

		/**
		 * Get some variable from the clause
		 * @returns 
		 *    0 if the clause is empty
		 *    some variable otherwise
		 */
		int getVar(void);
		std::string toString(void);

		/***** Mutators *****/
		int assign(int);
		int unassign(std::unordered_set<int>*);

		/***** Utility Functions *****/
		static int parseDimacs(CNF_Clause**, std::string);
};