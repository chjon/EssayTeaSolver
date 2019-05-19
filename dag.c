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