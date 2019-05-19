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

int dag_print_connect(const char* connective, dag_node_t* left, dag_node_t* right) {
	if (left == NULL || right == NULL) {
		return 1;
	}

	printf("(");
	if (dag_print(left)) return 1;
	printf(")%s(", connective);
	if (dag_print(right)) return 1;
	printf(")");
	return 0;
}

int dag_print_helper(dag_node_t* node) {
	if (node == NULL) {
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