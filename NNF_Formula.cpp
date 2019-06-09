#include "NNF_Formula.h"
#include <errno.h>
#include <fstream>
#include <sstream>

/***** CONSTRUCTORS *****/

NNF_Formula::NNF_Formula(int var) {
	this->var   = var;
	this->conn  = 0;
	this->left  = NULL;
	this->right = NULL;
}

NNF_Formula::NNF_Formula(char conn, NNF_Formula* left, NNF_Formula* right) {
	this->var   = 0;
	this->conn  = conn;
	this->left  = left;
	this->right = right;
}

NNF_Formula::~NNF_Formula(void) {

}

/***** MEMBER FUNCTIONS *****/

std::string NNF_Formula::toString(void) {
	std::string s = "";
	return s;
}

/***** UTILITY FUNCTIONS *****/

bool NNF_Formula::shouldPop(std::stack<char>* outputStack, char curr) {
	if (outputStack->size() == 0) {
		return false;
	}

	char top = outputStack->top();
	return (curr == '+' && top == '+') || (top == '.');
}

bool NNF_Formula::isOperator(char candidate) {
	return candidate == NNF_OR || candidate == NNF_AND;
}

bool NNF_Formula::isDigit(char candidate) {
	return candidate >= '0' && candidate <= '9';
}

bool NNF_Formula::isWhitespace(char candidate) {
	return
		candidate == ' '  ||
		candidate == '\t' ||
		candidate == '\r' ||
		candidate == '\n';
}

std::string NNF_Formula::reversePolishErrorMessage(int errcode) {
	std::string errMsg = "Error while parsing input: ";
	switch (errcode) {
		case -1:
			errMsg += "Misplaced negation";
			return errMsg;
		case -2:
			errMsg += "Missing operand";
			return errMsg;
		case -3:
			errMsg += "Misplaced parenthesis";
			return errMsg;
		case -4:
			errMsg +=  "Unbalanced parentheses";
			return errMsg;
		case -5:
			errMsg += "Invalid token";
			return errMsg;
		default:
			return errMsg;
	}
}

int NNF_Formula::toReversePolish(std::string* output, std::string input) {
	*output = "";
	std::stack<char> operatorStack;

	char prev = NNF_NONE;
	for (unsigned int i = 0; i < input.length(); i++) {
		char curr = input[i];
		if (isDigit(curr)) {
			if (isOperator(prev)) {
				output->push_back(NNF_REVERSE_POLISH_SEPARATOR);
			}

			output->push_back(curr);
			prev = curr;
		} else if (curr == NNF_NOT) {
			if (isOperator(prev)) {
				output->push_back(NNF_REVERSE_POLISH_SEPARATOR);
			}

			if (prev == NNF_NONE || isOperator(prev)) {
				output->push_back(curr);
				prev = curr;
			} else {
				errno = -1;
				return errno;
			}
		} else if (isOperator(curr)) {
			if (prev == NNF_NONE || prev == NNF_NOT || isOperator(prev)) {
				errno = -2;
				return errno;
			}

			while (shouldPop(&operatorStack, curr)) {
				output->push_back(NNF_REVERSE_POLISH_SEPARATOR);
				output->push_back(operatorStack.top());
				operatorStack.pop();
			}

			operatorStack.push(curr);
			prev = curr;
		} else if (curr == '(') {
			if (isDigit(prev)) {
				errno = -3;
				return errno;
			}
			operatorStack.push(curr);
		} else if (curr == ')') {
			while (operatorStack.size() && operatorStack.top() != '(') {
				output->push_back(NNF_REVERSE_POLISH_SEPARATOR);
				output->push_back(operatorStack.top());
				operatorStack.pop();
			}

			if (operatorStack.size() == 0) {
				errno = -4;
				return errno;
			} else {
				operatorStack.pop();
			}
		} else if (!isWhitespace(curr)) {
			errno = -5;
			return errno;
		}
	}

	if (prev == NNF_NOT || isOperator(prev)) {
		errno = -2;
		return errno;
	}

	while (operatorStack.size()) {
		if (operatorStack.top() == '(') {
			errno = -4;
			return errno;
		}

		output->push_back(NNF_REVERSE_POLISH_SEPARATOR);
		output->push_back(operatorStack.top());
		operatorStack.pop();
	}

	return 0;
}

int NNF_Formula::parseFile(NNF_Formula** formula, std::string pathname) {
	std::ifstream file;
	*formula = NULL;
	file.open(pathname);
	if (file.fail()) {
		errno = -1;
		return errno;
	}

	/* Concatenate formula */
	std::string formulaString = "";
	std::string line;
	while (std::getline(file, line)) {
		formulaString += line;
	}

	/* Convert to reverse Polish notation */
	std::string reversePolish;
	if (toReversePolish(&reversePolish, line)) {
		errno = -2;
		return errno;
	}

	/* Generate NNF structure */
	std::stack<NNF_Formula*> formulaStack;
	std::istringstream stream(reversePolish);
	while (!stream.eof()) {
		char next = stream.peek();
		int var;
		stream >> var;
		NNF_Formula* node = NULL;

		if (isOperator(next)) {
			NNF_Formula* right = formulaStack.top();
			formulaStack.pop();
			NNF_Formula* left = formulaStack.top();
			formulaStack.pop();
			node = new NNF_Formula(next, left, right);
		} else {
			node = new NNF_Formula(var);
		}

		formulaStack.push(node);
	}

	*formula = formulaStack.top();
	formulaStack.pop();

	file.close();
	return 0;
}

int NNF_Formula::tseitin(CNF_Formula** formulaCNF, NNF_Formula* formulaNNF) {
	*formulaCNF = NULL;
	return 0;
}