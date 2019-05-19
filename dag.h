#pragma once

/***** DEFINES *****/

#define DAG_TYPE_CONSTANT 0
#define DAG_TYPE_VARIABLE 1
#define DAG_TYPE_CONNECT  2

#define DAG_CONNECT_NOT   0
#define DAG_CONNECT_OR    1
#define DAG_CONNECT_AND   2
#define DAG_CONNECT_IMPLY 3
#define DAG_CONNECT_IFF   4

/***** TYPEDEFS *****/

typedef struct dag_node_t {
	int type;
	int value;
	struct dag_node_t* left;
	struct dag_node_t* right;
} dag_node_t;

/***** FUNCTIONS *****/

dag_node_t* dag_new(int, int, struct dag_node_t*, struct dag_node_t*);
/* void dag_print(dag_node_t*); */
