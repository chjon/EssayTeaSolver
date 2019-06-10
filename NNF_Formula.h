#pragma once
#include <string>
#include <stack>
#define NNF_SEPARATOR ' '
#define NNF_AND '.'
#define NNF_OR  '+'
#define NNF_NOT '-'
#define NNF_NONE '\0'

class CNF_Formula;

class NNF_Formula {
	private:
		int var;
		char conn;
		const unsigned int size;
		NNF_Formula* left;
		NNF_Formula* right;
		std::string stringRep;

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
		 */
		static int toReversePolish(std::string*, std::string);
		static std::string reversePolishErrorMessage(int);
		
		static int tseitinHelper(NNF_Formula*, int*, CNF_Formula**);

	public:
		~NNF_Formula(void);
		std::string toString(void);

		static int parseFile(NNF_Formula**, std::string);
		static int tseitinTransform(CNF_Formula**, NNF_Formula*);
};
