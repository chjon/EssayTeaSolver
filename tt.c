#include <stdlib.h>
#include "connectives.h"
#include "tt.h"

int tt_equivalent(dag_node_t* node, dag_node_t** node_list, int* index) {
	if (node == NULL) {
		return -1;
	}

	dag_node_t* left  = NULL;
	dag_node_t* right = NULL;

	if (node->type == BOOL_LOGIC_TYPE_CONNECT) {
		int index_left  = tt_equivalent(node->left,  node_list, index);
		int index_right = tt_equivalent(node->right, node_list, index);
		left  = node_list[index_left ];
		right = node_list[index_right];
	}

	switch (node->type) {
	case BOOL_LOGIC_TYPE_CONSTANT:
	case BOOL_LOGIC_TYPE_CONNECT:
		node_list[*index] = dag_new(BOOL_LOGIC_TYPE_CONSTANT, node->value, left, right);
		break;
	case BOOL_LOGIC_TYPE_VARIABLE:
		node_list[*index] = dag_new(BOOL_LOGIC_TYPE_VARIABLE, *index, NULL, NULL);
		break;
	}

	int index_node = *index;
	++*index;
	return index_node;
}

dag_node_t* tt_transform(dag_node_t* node) {
	/* Step 1: Assign a new variable to each subformula */
	/* Implementation note: we will index by post order traversal */
	int num_nodes = dag_count(node);
	dag_node_t** transformed_nodes = malloc(num_nodes * sizeof(dag_node_t));

	/* Step 2: Set representatives to be equivalent */
	int index = 0;
	tt_equivalent(node, transformed_nodes, &index);
}