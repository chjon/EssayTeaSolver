#pragma once

#include "dag.h"

dag_node_t* nf_cnf(dag_node_t*);
dag_node_t* nf_dnf(dag_node_t*);
dag_node_t* nf_nnf(dag_node_t*);