#include "NNF_Formula.h"
#include <errno.h>
#include <fstream>

/***** CONSTRUCTORS *****/

NNF_Formula::NNF_Formula(int var) {
	this->var  = var;
	this->conn = NNF_NONE;
}

NNF_Formula::NNF_Formula(NNF_Connective conn) {
	this->var  = 0;
	this->conn = conn;
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

int NNF_Formula::toReversePolish(std::string* output, std::string input) {
	*output = "";
	std::stack<char> operatorStack;

	char prev = 0;
	for (unsigned int i = 0; i < input.length(); i++) {
		char curr = input[i];
		if (curr >= '0' && curr <= '9') {
			if (prev == '+' || prev == '.') {
				output->push_back(',');
			}

			output->push_back(curr);
			prev = curr;
		} else if (curr == '-') {
			if (prev == '+' || prev == '.') {
				output->push_back(',');
			}

			if (prev == 0 || prev == '+' || prev == '.') {
				output->push_back(curr);
				prev = curr;
			} else {
				errno = -1;
				return errno;
			}
		} else if (curr == '+' || curr == '.') {
			if (prev == 0 || prev == '-' || prev == '+' || prev == '.') {
				errno = -2;
				return errno;
			}

			while (shouldPop(&operatorStack, curr)) {
				output->push_back(',');
				output->push_back(operatorStack.top());
				operatorStack.pop();
			}

			operatorStack.push(curr);
			prev = curr;
		} else if (curr == '(') {
			if (prev >= '0' && prev <= '9') {
				errno = -3;
				return errno;
			}
			operatorStack.push(curr);
		} else if (curr == ')') {
			while (operatorStack.size() && operatorStack.top() != '(') {
				output->push_back(',');
				output->push_back(operatorStack.top());
				operatorStack.pop();
			}

			if (operatorStack.size() == 0) {
				errno = -4;
				return errno;
			} else {
				operatorStack.pop();
			}
		} else if (curr != ' ' && curr != '\t' && curr != '\n' && curr != '\r') {
			errno = -5;
			return errno;
		}
	}

	if (prev == '-' || prev == '+' || prev == '.') {
		errno = -2;
		return errno;
	}

	while (operatorStack.size()) {
		if (operatorStack.top() == '(') {
			errno = -4;
			return errno;
		}

		output->push_back(',');
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
	for (unsigned int i = 0; i < formulaString.length(); i++) {

	}

	file.close();
	return 0;
}

int NNF_Formula::tseitin(CNF_Formula** formulaCNF, NNF_Formula* formulaNNF) {
	*formulaCNF = NULL;
	return 0;
}