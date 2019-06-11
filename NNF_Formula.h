#pragma once
#include <unordered_map>
#include <stack>
#include <string>
#define NNF_SEPARATOR ' '
#define NNF_AND '.'
#define NNF_OR  '+'
#define NNF_NOT '-'
#define NNF_NONE '\0'
#define NNF_INVARIANT (0x4243)

class CNF_Formula;

class NNF_Formula {
	private:
		int var;
		char conn;
		NNF_Formula* left;
		NNF_Formula* right;
		std::string stringRep;
		int invariant;

		NNF_Formula(int);
		NNF_Formula(char, NNF_Formula*, NNF_Formula*);

		static bool shouldPop(std::stack<char>*, char);
		static bool isOperator(char);
		static bool isDigit(char);
		static bool isWhitespace(char);

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
		 *   -6 No input
		 */
		static int toReversePolish(std::string*, std::string);
		static std::string reversePolishErrorMessage(int);
		
		static CNF_Formula* generateLocalCNF(
			NNF_Formula*,
			std::unordered_map<std::string, int>*
		);
		
		static int reduceRedundancies(
			NNF_Formula*,
			std::unordered_map<std::string, NNF_Formula*>*,
			int*,
			std::unordered_map<std::string, int>*
		);

	public:
		~NNF_Formula(void);
		std::string toString(void);

		static int parseFile(NNF_Formula**, std::string);
		static int tseitinTransform(CNF_Formula**, NNF_Formula*);
};
