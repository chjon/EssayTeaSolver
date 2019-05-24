#include <stdio.h>
#include <stdlib.h>
#include "dag.h"
#include "nf.h"

int main(const int argc, const char** argv) {
	if (argc != 2) {
		printf("Usage: %s <boolean formula>\n", argv[0]);
		return -1;
	}

	dag_node_t* dag = dag_parse(argv[1]);
	dag_print(dag);
	dag = nf_nnf(dag);
	dag_print(dag);
	dag_delete(dag);

	return 0;
}