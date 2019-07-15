#pragma once
#include "CNF_Formula.h"
#include <unordered_set>

class DPLL {
	private:
		/**
		 * Boolean constraint propagation: perform unit resolution until saturation
		 * @returns
		 *    -1: reduced to contradiction
		 *     0: success
		 */
		static int bcp(
			std::unordered_set<CNF_Clause*>*,
			std::unordered_set<int>*,
			std::unordered_set<int>*
		);

		/**
		 * Pure literal propagation: assign literals that only appear positively or negatively
		 * @returns
		 *     std::unordered_set<int>* - variables that were assigned by PLP
		 */
		static std::unordered_set<int>* plp(std::unordered_set<CNF_Clause*>*, std::unordered_set<int>*);

		/**
		 * Undo variable assignments
		 */
		static void undoAssignments(
			std::unordered_set<CNF_Clause*>*,
			std::unordered_set<int>*,
			std::unordered_set<int>*
		);

		/**
		 * Main DPLL algorithm
		 */
		static bool dpllHelper(std::unordered_set<CNF_Clause*>*, std::unordered_set<int>*, int);

	public:
		static std::unordered_set<int>* dpll(CNF_Formula*);
};
