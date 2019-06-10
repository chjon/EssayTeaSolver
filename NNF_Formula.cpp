#include "NNF_Formula.h"
#include "CNF_Formula.h"
#include "CNF_Clause.h"
#include <errno.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

/***** CONSTRUCTORS *****/

NNF_Formula::NNF_Formula(int var) :
	var{var},
	conn{0},
	left{NULL},
	right{NULL},
	stringRep{std::to_string(var)},
	invariant{NNF_INVARIANT}
{}

NNF_Formula::NNF_Formula(char conn, NNF_Formula* left, NNF_Formula* right) :
	var{0},
	conn{conn},
	left{NULL},
	right{NULL},
	invariant{NNF_INVARIANT}
{
	if (right->stringRep.compare(left->stringRep) < 0) {
		NNF_Formula* tmp = left;
		left = right;
		right = tmp;
	}

	this->left = left;
	this->right = right;
	stringRep = left->stringRep + conn + right->stringRep;
}

NNF_Formula::~NNF_Formula(void) {
	invariant = 0;

	if (conn) {
		if (left->invariant == NNF_INVARIANT) {
			delete left;
		}
		if (right->invariant == NNF_INVARIANT) {
			delete right;
		}
	}
}

/***** MEMBER FUNCTIONS *****/

std::string NNF_Formula::toString(void) {
	if (isOperator(conn)) {
		return "(" + left->toString() + ")" + conn + "(" + right->toString() + ")";
	} else {
		return std::to_string(var);
	}
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
				output->push_back(NNF_SEPARATOR);
			}

			output->push_back(curr);
			prev = curr;
		} else if (curr == NNF_NOT) {
			if (isOperator(prev)) {
				output->push_back(NNF_SEPARATOR);
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
				output->push_back(NNF_SEPARATOR);
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
				output->push_back(NNF_SEPARATOR);
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

		output->push_back(NNF_SEPARATOR);
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
	if (toReversePolish(&reversePolish, formulaString)) {
		std::cout << NNF_Formula::reversePolishErrorMessage(errno) << std::endl;
		errno = -2;
		return errno;
	}

	/* Generate NNF structure */
	std::stack<NNF_Formula*> formulaStack;
	bool negative = false;
	int val = 0;
	char curr;
	for (unsigned int i = 0; i < reversePolish.length(); i++) {
		curr = reversePolish[i];
		if (curr == NNF_SEPARATOR && val) {
			if (negative) {
				val *= -1;
			}

			formulaStack.push(new NNF_Formula(val));
			negative = false;
			val = 0;
		} else if (isDigit(curr)) {
			val = val * 10 + curr - '0';
		} else if (curr == NNF_NOT) {
			negative = true;
		} else if (isOperator(curr)) {
			NNF_Formula* right = formulaStack.top();
			formulaStack.pop();
			NNF_Formula* left = formulaStack.top();
			formulaStack.pop();
			formulaStack.push(new NNF_Formula(curr, left, right));
		}
	}

	*formula = formulaStack.top();
	formulaStack.pop();

	file.close();
	return 0;
}

int NNF_Formula::reduceRedundancies(
	NNF_Formula* formula,
	std::unordered_map<std::string, NNF_Formula*>* formulaMap,
	int* index,
	std::unordered_map<std::string, int>* indexMap
) {
	if (isOperator(formula->conn)) {
		std::unordered_map<std::string, NNF_Formula*>::iterator found =
			formulaMap->find(formula->left->stringRep);
		if (found != formulaMap->end()) {
			delete formula->left;
			formula->left = found->second;
		} else {
			reduceRedundancies(formula->left, formulaMap, index, indexMap);
		}

		found = formulaMap->find(formula->right->stringRep);
		if (found != formulaMap->end()) {
			delete formula->right;
			formula->right = found->second;
		} else {
			reduceRedundancies(formula->right, formulaMap, index, indexMap);
		}

		indexMap->insert(std::pair<std::string, int>(formula->stringRep, *index));
		++*index;
	} else {
		int absVar = (formula->var > 0) ? (formula->var) : (-formula->var);
		std::unordered_map<std::string, int>::iterator found =
			indexMap->find(std::to_string(absVar));
		
		if (found == indexMap->end()) {
			indexMap->insert(std::pair<std::string, int>(std::to_string(absVar), *index));
			indexMap->insert(std::pair<std::string, int>(std::to_string(-absVar), -*index));
			++*index;
		}
	}

	formulaMap->insert(std::pair<std::string, NNF_Formula*>(formula->stringRep, formula));
	return 0;
}

CNF_Formula* NNF_Formula::generateLocalCNF(
	NNF_Formula* formulaNNF,
	std::unordered_map<std::string, int>* indexMap
) {
	if (isOperator(formulaNNF->conn)) {
		std::unordered_set<CNF_Clause*>* clauses = new std::unordered_set<CNF_Clause*>();
		std::unordered_set<int>* vars;

		int index      = indexMap->find(formulaNNF->stringRep)->second;
		int leftIndex  = indexMap->find(formulaNNF->left->stringRep)->second;
		int rightIndex = indexMap->find(formulaNNF->right->stringRep)->second;

		if (formulaNNF->conn == NNF_OR) {
			vars = new std::unordered_set<int>();
			vars->insert(index);
			vars->insert(-leftIndex);
			clauses->insert(new CNF_Clause(vars));
			
			vars = new std::unordered_set<int>();
			vars->insert(index);
			vars->insert(-rightIndex);
			clauses->insert(new CNF_Clause(vars));

			vars = new std::unordered_set<int>();
			vars->insert(-index);
			vars->insert(leftIndex);
			vars->insert(rightIndex);
			clauses->insert(new CNF_Clause(vars));

		} else if (formulaNNF->conn == NNF_AND) {
			vars = new std::unordered_set<int>();
			vars->insert(-index);
			vars->insert(leftIndex);
			clauses->insert(new CNF_Clause(vars));

			vars = new std::unordered_set<int>();
			vars->insert(-index);
			vars->insert(rightIndex);
			clauses->insert(new CNF_Clause(vars));

			vars = new std::unordered_set<int>();
			vars->insert(index);
			vars->insert(-leftIndex);
			vars->insert(-rightIndex);
			clauses->insert(new CNF_Clause(vars));
		} else {
			delete clauses;
			return NULL;
		}

		return new CNF_Formula(clauses);
	} else {
		return NULL;
	}
}

int NNF_Formula::tseitinTransform(CNF_Formula** formulaCNF, NNF_Formula* formulaNNF) {
	*formulaCNF = NULL;
	std::unordered_map<std::string, NNF_Formula*>* formulaMap;
	std::unordered_map<std::string, int>* indexMap;
	formulaMap = new std::unordered_map<std::string, NNF_Formula*>();
	indexMap = new std::unordered_map<std::string, int>();

	/* Minimize redundant nodes */
	int index = 1;
	if (reduceRedundancies(formulaNNF, formulaMap, &index, indexMap)) {
		delete formulaMap;
		delete indexMap;
		errno = -1;
		return -1;
	}

	/* Generate local CNF formulae */
	CNF_Formula** subformulae = new CNF_Formula*[indexMap->size()];
	index = 0;
	for (
		std::unordered_map<std::string, NNF_Formula*>::iterator i = formulaMap->begin();
		i != formulaMap->end();
		++i, ++index
	) {
		NNF_Formula* subformula = i->second;
		subformulae[index] = generateLocalCNF(subformula, indexMap);
	}

	if (CNF_Formula::combine(formulaCNF, subformulae, index)) {
		delete[] subformulae;
		errno = -3;
		return errno;
	}

	delete[] subformulae;
	return 0;
}