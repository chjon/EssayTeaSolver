#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

int main(const int argc, const char** argv) {
	dag_node_t* dag = dag_new(0, 0, NULL, NULL);
	dag_print(dag);
	dag->value = 1;
	dag_print(dag);
	dag->type = 1;
	dag->value = 0;
	dag_print(dag);
	dag->value = 1;
	dag_print(dag);
	dag->value = 2;
	dag_print(dag);

	return 0;
}