#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

int main(const int argc, const char** argv) {
	dag_node_t* dag = dag_parse("x_1&((x_0)|(x_2))");
	dag_print(dag);
	dag_delete(dag);

	return 0;
}