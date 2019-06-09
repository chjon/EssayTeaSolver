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
		static std::string reversePolishErrorMessage(int);
		/**
		 * Converts an input formula to reverse Polish notation
		 * 
		 * @returns
		 *    0 Success
		 *   -1 Misplaced negation
		 *   -2 Missing operand
		 *   -3 Misplaced parenthesis
		 *   -4 Unbalanced parentheses
		 *   -5 Invalid token
		 */
		static int toReversePolish(std::string*, std::string);
		~NNF_Formula(void);

		std::string toString(void);

		static int parseFile(NNF_Formula**, std::string);
		static int tseitin(CNF_Formula**, NNF_Formula*);
};
