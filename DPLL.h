#include "CNF_Formula.h"
#include <unordered_set>
#include <vector>

class DPLL {
	public:
		/**
		 * Boolean constraint propagation: perform unit resolution until saturation
		 * @returns
		 *     NULL - contradiction
		 *     std::unordered_set<int>* - variables that were assigned by BCP
		 */
		static std::unordered_set<int>* bcp(std::vector<CNF_Clause*>*, std::unordered_set<int>*);

		/**
		 * Pure literal propagation: assign literals that only appear positively or negatively
		 * @returns
		 *     std::unordered_set<int>* - variables that were assigned by PLP
		 */
		static std::unordered_set<int>* plp(std::vector<CNF_Clause*>*, std::unordered_set<int>*);

		/**
		 * Undo variable assignments
		 */
		static void undoAssignments(std::vector<CNF_Clause*>*, std::unordered_set<int>*);
};
