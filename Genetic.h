#pragma once
#include <vector>
#include <cmath>

#include "Graph.h"

namespace genetic {
	class Chromosome {
	private:
		std::vector<bool> included;
		Graph* basicGraph;
		int getEdgeCount();
	public:
		Graph getGraph();
		int getFitness();
		Chromosome(Graph& graph);
	};
}