#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

dag_node_t* dag_new(int type, int value, struct dag_node_t* left, struct dag_node_t* right) {
	dag_node_t* dag = malloc(sizeof(struct dag_node_t));
	dag->type  = type;
	dag->value = value;
	dag->left  = left;
	dag->right = right;
	return dag;
}

dag_node_t* dag_parse_variable(const char* str, int* index) {
	dag_node_t* node = NULL;

	if (
		str[  *index] == 'x' &&
		str[++*index] == '_' &&
		str[++*index] >= '0' &&
		str[  *index] <= '9'
	) {
		int num = 0;
		while (str[*index] >= '0' && str[*index] <= '9') {
			num *= ('9' - '0' + 1);
			num += str[*index] - '0';
			++*index;
		}
		node = dag_new(DAG_TYPE_VARIABLE, num, NULL, NULL);
	}

	return node;
}

dag_node_t* dag_parse_helper(const char* str, int* index, int level) {
	enum state { PARSE_LEFT, PARSE_CONNECTIVE, PARSE_RIGHT, COMPLETE, ERROR };
	enum state current_state = PARSE_LEFT;

	if (str == NULL || level < 0 || !str[*index]) {
		return NULL;
	}

	dag_node_t* node  = NULL;
	dag_node_t* left  = NULL;
	dag_node_t* right = NULL;
	int type = -1;
	int value = -1;

	while (str[*index] && current_state != ERROR && current_state != COMPLETE) {
		if (str[*index] == '(') {
			++*index;
			switch (current_state) {
			case PARSE_LEFT:
				left = dag_parse_helper(str, index, level + 1);
				if (left == NULL || str[*index] != ')') {
					current_state = ERROR;
				} else {
					++*index;
					current_state = PARSE_CONNECTIVE;
				}
				break;
			case PARSE_RIGHT:
				right = dag_parse_helper(str, index, level + 1);
				if (right == NULL || str[*index] != ')') {
					current_state = ERROR;
				} else {
					++*index;
					current_state = COMPLETE;
				}
				break;
			default:
				current_state = ERROR;
				break;
			}
		} else if (str[*index] == 'x') {
			switch (current_state) {
				case PARSE_LEFT:
					left = dag_parse_variable(str, index);
					if (left == NULL) {
						current_state = ERROR;
					} else {
						current_state = PARSE_CONNECTIVE;
					}
					break;
				case PARSE_RIGHT:
					right = dag_parse_variable(str, index);
					if (right == NULL) {
						current_state = ERROR;
					} else {
						current_state = COMPLETE;
					}
					break;
				default:
					current_state = ERROR;
					break; 
			}
		} else if (str[*index] == '!') {
			if (current_state == PARSE_LEFT) {
				type = DAG_TYPE_CONNECT;
				value = DAG_CONNECT_NOT;
				current_state = PARSE_RIGHT;
			} else {
				current_state = ERROR;
			}
		} else if (current_state == PARSE_CONNECTIVE) {
			current_state = PARSE_RIGHT;

			switch (str[*index]) {
			case ')':
				current_state = COMPLETE;
				break;
			case '|':
				type = DAG_TYPE_CONNECT;
				value = DAG_CONNECT_OR;
				++*index;
				break;
			case '&':
				type = DAG_TYPE_CONNECT;
				value = DAG_CONNECT_AND;
				++*index;
				break;
			case '-':
				if (str[++*index] == '>') {
					type = DAG_TYPE_CONNECT;
					value = DAG_CONNECT_IMPLY;
				} else {
					current_state = ERROR;
				}
				break;
			case '<':
				if (
					str[++*index] == '-' &&
					str[++*index] == '>'
				) {
					type = DAG_TYPE_CONNECT;
					value = DAG_CONNECT_IFF;
				} else {
					current_state = ERROR;
				}
				break;
			default:
				current_state = ERROR;
				break;
			}
		} else {
			current_state = ERROR;
		}
	}

	if (!str[*index] && current_state == PARSE_CONNECTIVE) {
		current_state = COMPLETE;
	}

	if (current_state == ERROR || (str[*index] && !level)) {
		if (left != NULL) dag_delete(left);
		if (right != NULL) dag_delete(right);
		return NULL;
	} else if (current_state == COMPLETE) {
		if (right == NULL) return left;
		if (left == NULL) left = right;
		node = dag_new(type, value, left, right);
		return node;
	} else {
		if (left != NULL) dag_delete(left);
		if (right != NULL) dag_delete(right);
		return NULL;
	}
}

dag_node_t* dag_parse(const char* str) {
	int index = 0;
	return dag_parse_helper(str, &index, 0);
}

void dag_delete(dag_node_t* node) {
	if (node == NULL) {
		return;
	}

	switch (node->type) {
	case DAG_TYPE_CONNECT:
		switch (node->value) {
		case DAG_CONNECT_OR:
		case DAG_CONNECT_AND:
		case DAG_CONNECT_IMPLY:
		case DAG_CONNECT_IFF:
			dag_delete(node->right);
		case DAG_CONNECT_NOT:
			dag_delete(node->left);
		default:
			break;
		}
		node->right = NULL;
		node->left  = NULL;
	case DAG_TYPE_CONSTANT:
	case DAG_TYPE_VARIABLE:
		free(node);
	default:
		return;
	}
}

int dag_print_helper(dag_node_t*);

int dag_print_connect(const char* connective, dag_node_t* left, dag_node_t* right) {
	if (left == NULL || right == NULL) {
		return 1;
	}

	printf("(");
	if (dag_print_helper(left)) return 1;
	printf(")%s(", connective);
	if (dag_print_helper(right)) return 1;
	printf(")");
	return 0;
}

int dag_print_helper(dag_node_t* node) {
	if (node == NULL) {
		printf("NULL");
		return 0;
	}

	switch (node->type) {
	case DAG_TYPE_CONSTANT:
		if (node->value) {
			printf("T");
		} else {
			printf("F");
		}
		return 0;
	case DAG_TYPE_VARIABLE:
		printf("x_%d", node->value);
		return 0;
	case DAG_TYPE_CONNECT:
		switch (node->value) {
			case DAG_CONNECT_NOT:
				printf("!");
				return dag_print(node->left);
			case DAG_CONNECT_OR:
				return dag_print_connect("|", node->left, node->right);
			case DAG_CONNECT_AND:
				return dag_print_connect("&", node->left, node->right);
			case DAG_CONNECT_IMPLY:
				return dag_print_connect("->", node->left, node->right);
			case DAG_CONNECT_IFF:
				return dag_print_connect("<->", node->left, node->right);
			default:
				return 1;
		}
	default:
		return 1;
	}
}

int dag_print(dag_node_t* node) {
	if (dag_print_helper(node)) return 1;
	printf("\n");
	return 0;
}