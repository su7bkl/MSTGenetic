#include "Genetic.h"

namespace genetic {
	Chromosome::Chromosome(Graph& graph) {
		this->included.resize(graph.getVeretexCount());
		this->basicGraph = graph;
	}

	Graph Chromosome::getGraph() {
		int edgeId = 0;
		std::vector<Edge> includedEdges;
		for (int i = 1; i < this->basicGraph.getVeretexCount(); i++) {
			for (int j = 0; j < i; j++) {
				if (this->basicGraph.getEgdeLength({ i, j }) != 0) {
					if (this->included[edgeId]) {
						includedEdges.push_back({ {i, j}, this->basicGraph.getEgdeLength({ i, j }) });
					}
					edgeId++;
				}
			}
		}
		return Graph(this->basicGraph.getVertices(), includedEdges);
	}

	int Chromosome::getFitness() {
		int edgeId = 0;
		int fitness = 0;
		for (int i = 1; i < this->basicGraph.getVeretexCount(); i++) {
			for (int j = 0; j < i; j++) {
				if (this->basicGraph.getEgdeLength({ i, j }) != 0) {
					if (this->included[edgeId]) {
						fitness += this->basicGraph.getEgdeLength({ i, j });
					}
					edgeId++;
				}
			}
		}
		fitness += (int)std::round(std::log(this->getGraph().getConnectedComponentsCount()) * this->basicGraph.getTotalEdgeLength());
		return fitness;
	}
}