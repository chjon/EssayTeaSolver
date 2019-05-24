#include <stdlib.h>
#include "dag.h"
#include "connectives.h"

dag_node_t* nf_cnf(dag_node_t* root) {
	return root;
}

dag_node_t* nf_dnf(dag_node_t* root) {
	return root;
}

dag_node_t* nf_nnf(dag_node_t* root) {
	if (root == NULL) {
		return NULL;
	}

	switch (root->type) {
		case BOOL_LOGIC_TYPE_CONSTANT:
		case BOOL_LOGIC_TYPE_VARIABLE:
			return root;
		case BOOL_LOGIC_TYPE_CONNECT:
			if (root->left == NULL || root->right == NULL) {
				return NULL;
			}

			if (root->value == BOOL_LOGIC_CONN_IMPLY) {
				root->value = BOOL_LOGIC_CONN_OR;
				root->left = dag_new(
					BOOL_LOGIC_TYPE_CONNECT,
					BOOL_LOGIC_CONN_NOT,
					root->left,
					root->left
				);
			} else if (root->value == BOOL_LOGIC_CONN_IFF) {
				dag_node_t* left = dag_new(
					BOOL_LOGIC_TYPE_CONNECT,
					BOOL_LOGIC_CONN_IMPLY,
					root->left,
					root->right
				);
				dag_node_t* right = dag_new(
					BOOL_LOGIC_TYPE_CONNECT,
					BOOL_LOGIC_CONN_IMPLY,
					root->right,
					root->left
				);
				root->left  = left;
				root->right = right;
			}

			if (root->value == BOOL_LOGIC_CONN_NOT) {
				if (root->left->type == BOOL_LOGIC_TYPE_CONNECT) {
					if (root->left->value == BOOL_LOGIC_CONN_NOT) {
						dag_node_t* temp = root->left->left;
						if (temp == NULL) {
							return NULL;
						}

						free(root->left);
						free(root);
						root = nf_nnf(temp);
					} else if (root->left->value == BOOL_LOGIC_CONN_OR) {
						root->left->value = BOOL_LOGIC_CONN_AND;
					} else if (root->left->value == BOOL_LOGIC_CONN_AND) {
						root->left->value = BOOL_LOGIC_CONN_OR;
					}

					if (
						root->left->value == BOOL_LOGIC_CONN_OR ||
						root->left->value == BOOL_LOGIC_CONN_AND
					) {
						dag_node_t* left = dag_new(
							BOOL_LOGIC_TYPE_CONNECT,
							BOOL_LOGIC_CONN_NOT,
							root->left->left,
							root->left->left
						);
						dag_node_t* right = dag_new(
							BOOL_LOGIC_TYPE_CONNECT,
							BOOL_LOGIC_CONN_NOT,
							root->left->right,
							root->left->right
						);
						root->left->left = left;
						root->left->right = right;

						dag_node_t* temp = root->left;
						free(root);
						root = temp;
					}
				}
			}

			if (
				root->value == BOOL_LOGIC_CONN_OR ||
				root->value == BOOL_LOGIC_CONN_AND
			) {
				root->left = nf_nnf(root->left);
				root->right = nf_nnf(root->right);
			}

			return root;
		default:
			return NULL;
	}

	return root;
}