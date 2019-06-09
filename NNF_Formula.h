#pragma once
#include <string>
#include <stack>

class CNF_Formula;

class NNF_Formula {
	private:
		enum NNF_Connective {
			NNF_AND,
			NNF_OR,
			NNF_NONE,
		};

		int var;
		NNF_Connective conn;

		NNF_Formula(int);
		NNF_Formula(NNF_Connective);

		static bool shouldPop(std::stack<char>*, char);

	public:
		/**
		 * Converts an input formula to reverse Polish notation
		 * 
		 * @returns
		 *    0 Success
		 *   -1 misplaced negation
		 *   -2 missing operand
		 *   -3 misplaced parenthesis
		 *   -4 unbalanced parentheses
		 */
		static int toReversePolish(std::string*, std::string);
		~NNF_Formula(void);

		std::string toString(void);

		static int parseFile(NNF_Formula**, std::string);
		static int tseitin(CNF_Formula**, NNF_Formula*);
};
