#pragma once

/***** TYPEDEFS *****/

typedef struct dag_node_t {
	int type;
	int value;
	struct dag_node_t* left;
	struct dag_node_t* right;
} dag_node_t;

/***** FUNCTIONS *****/

dag_node_t* dag_new(int, int, dag_node_t*, dag_node_t*);
dag_node_t* dag_parse(const char*);
void dag_delete(dag_node_t*);

int dag_print(dag_node_t*);
int dag_count(dag_node_t*);