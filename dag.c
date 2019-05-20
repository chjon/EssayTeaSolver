#include <stdio.h>
#include <stdlib.h>
#include "connectives.h"
#include "dag.h"

dag_node_t* dag_new(int type, int value, struct dag_node_t* left, struct dag_node_t* right) {
	dag_node_t* dag = malloc(sizeof(struct dag_node_t));
	dag->type  = type;
	dag->value = value;
	dag->left  = left;
	dag->right = right;
	return dag;
}

dag_node_t* dag_parse_helper(const char*, int*, int);
dag_node_t* dag_parse_constant(const char*, int*);
dag_node_t* dag_parse_variable(const char*, int*);
dag_node_t* dag_parse_bracket(const char*, int*, int);
int begins_with(const char*, int, const char*);

dag_node_t* dag_parse(const char* str) {
	int index = 0;
	return dag_parse_helper(str, &index, 0);
}

void dag_delete(dag_node_t* node) {
	if (node == NULL) {
		return;
	}

	switch (node->type) {
	case BOOL_LOGIC_TYPE_CONNECT:
		switch (node->value) {
		case BOOL_LOGIC_CONN_OR:
		case BOOL_LOGIC_CONN_AND:
		case BOOL_LOGIC_CONN_IMPLY:
		case BOOL_LOGIC_CONN_IFF:
			dag_delete(node->right);
		case BOOL_LOGIC_CONN_NOT:
			dag_delete(node->left);
		default:
			break;
		}
		node->right = NULL;
		node->left  = NULL;
	case BOOL_LOGIC_TYPE_CONSTANT:
	case BOOL_LOGIC_TYPE_VARIABLE:
		free(node);
	default:
		return;
	}
}

int dag_print_helper(dag_node_t*);
int dag_print_connect(const char*, dag_node_t*, dag_node_t*);

int dag_print(dag_node_t* node) {
	if (dag_print_helper(node)) return 1;
	printf("\n");
	return 0;
}

/***** HELPER FUNCTIONS *****/

dag_node_t* dag_parse_constant(const char* str, int* index) {
	dag_node_t* node = NULL;

	if (str[*index] == 'T') {
		node = dag_new(BOOL_LOGIC_TYPE_CONSTANT, 1, NULL, NULL);
	} else if (str[*index] == 'F') {
		node = dag_new(BOOL_LOGIC_TYPE_CONSTANT, 0, NULL, NULL);
	}

	++*index;
	return node;
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
		node = dag_new(BOOL_LOGIC_TYPE_VARIABLE, num, NULL, NULL);
	}

	return node;
}

dag_node_t* dag_parse_bracket(const char* str, int* index, int level) {
	if (str[*index] != '(') {
		return NULL;
	}

	++*index;
	dag_node_t* parsed = dag_parse_helper(str, index, level);
	if (parsed == NULL || str[*index] != ')') {
		dag_delete(parsed);
		return NULL;
	}

	++*index;
	return parsed;
}

int begins_with(const char* str, int base, const char* pre) {
	int i = 0;
	while (pre[i] && str[base + i] == pre[i]) {
		++i;
	}

	return (pre[i]) ? 0 : i;
}

dag_node_t* dag_parse_helper(const char* str, int* index, int level) {
	enum state { PARSE_LEFT, PARSE_CONNECTIVE, PARSE_RIGHT, COMPLETE, ERROR };
	enum state current_state = PARSE_LEFT;
	dag_node_t* parsed = NULL;
	dag_node_t* left   = NULL;
	dag_node_t* right  = NULL;
	int type = -1;
	int value = -1;

	if (str == NULL || !str[*index]) {
		return NULL;
	}

	while (str[*index] && current_state != ERROR && current_state != COMPLETE) {
		if (current_state == PARSE_CONNECTIVE) {
			if (str[*index] == ')') {
				current_state = COMPLETE;
				continue;
			}

			int len = 0;
			if ((len = begins_with(str, *index, BOOL_LOGIC_SYM_OR))) {
				value = BOOL_LOGIC_CONN_OR;
			} else if ((len = begins_with(str, *index, BOOL_LOGIC_SYM_AND))) {
				value = BOOL_LOGIC_CONN_AND;
			} else if ((len = begins_with(str, *index, BOOL_LOGIC_SYM_IMPLY))) {
				value = BOOL_LOGIC_CONN_IMPLY;
			} else if ((len = begins_with(str, *index, BOOL_LOGIC_SYM_IFF))) {
				value = BOOL_LOGIC_CONN_IFF;
			}

			if (value == -1) {
				current_state = ERROR;
			}

			*index += len;
			current_state = PARSE_RIGHT;
			type = BOOL_LOGIC_TYPE_CONNECT;

			continue;
		}
		
		int negate = (begins_with(str, *index, BOOL_LOGIC_SYM_NOT));
		parsed = NULL;

		if (negate) ++*index;

		if (str[*index] == 'T' || str[*index] == 'F') {
			parsed = dag_parse_constant(str, index);
		} else if (str[*index] == 'x') {
			parsed = dag_parse_variable(str, index);
		} else if (str[*index] == '(') {
			parsed = dag_parse_bracket(str, index, level + 1);
		}

		if (parsed == NULL) {
			current_state = ERROR;
			continue;
		}

		if (negate) {
			parsed = dag_new(BOOL_LOGIC_TYPE_CONNECT, BOOL_LOGIC_CONN_NOT, parsed, parsed);
		}

		if (current_state == PARSE_LEFT) {
			left = parsed;
			current_state = PARSE_CONNECTIVE;
		} else {
			right = parsed;
			current_state = COMPLETE;
		}
	}

	if (!str[*index] && current_state == PARSE_CONNECTIVE) {
		current_state = COMPLETE;
	}

	if (current_state == COMPLETE && (!str[*index] || level)) {
		if (right == NULL) return left;
		if (left == NULL) left = right;
		return dag_new(type, value, left, right);
	} else {
		if (left != NULL) dag_delete(left);
		if (right != NULL) dag_delete(right);
		return NULL;
	}
}

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
	case BOOL_LOGIC_TYPE_CONSTANT:
		if (node->value) {
			printf("T");
		} else {
			printf("F");
		}
		return 0;
	case BOOL_LOGIC_TYPE_VARIABLE:
		printf("x_%d", node->value);
		return 0;
	case BOOL_LOGIC_TYPE_CONNECT:
		switch (node->value) {
			case BOOL_LOGIC_CONN_NOT:
				printf("%s(", BOOL_LOGIC_SYM_NOT);
				int to_return = dag_print_helper(node->left);
				printf(")");
				return to_return;
			case BOOL_LOGIC_CONN_OR:
				return dag_print_connect(BOOL_LOGIC_SYM_OR, node->left, node->right);
			case BOOL_LOGIC_CONN_AND:
				return dag_print_connect(BOOL_LOGIC_SYM_AND, node->left, node->right);
			case BOOL_LOGIC_CONN_IMPLY:
				return dag_print_connect(BOOL_LOGIC_SYM_IMPLY, node->left, node->right);
			case BOOL_LOGIC_CONN_IFF:
				return dag_print_connect(BOOL_LOGIC_SYM_IFF, node->left, node->right);
			default:
				return 1;
		}
	default:
		return 1;
	}
}