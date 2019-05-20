#pragma once

/***** ENUMS *****/

enum dag_type {
	DAG_TYPE_CONSTANT,
	DAG_TYPE_VARIABLE,
	DAG_TYPE_CONNECT,
};

enum dag_connect {
	DAG_CONNECT_NOT,
	DAG_CONNECT_OR,
	DAG_CONNECT_AND,
	DAG_CONNECT_IMPLY,
	DAG_CONNECT_IFF,
};

/***** TYPEDEFS *****/

typedef struct dag_node_t {
	int type;
	int value;
	struct dag_node_t* left;
	struct dag_node_t* right;
} dag_node_t;

/***** FUNCTIONS *****/

dag_node_t* dag_new(int, int, struct dag_node_t*, struct dag_node_t*);
dag_node_t* dag_parse(const char*);
void dag_delete(dag_node_t*);

int dag_print(dag_node_t*);
