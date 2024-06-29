#include "Graph.h"
#include <algorithm>

Graph::Graph(int vertices) : vertices(vertices, Vertex()), adjacencyMatrix(vertices, std::vector(vertices, 0))
{
}

int Graph::getVerticesCount() const
{
	return this->vertices.size();
}

void Graph::setVerticesCount(int value)
{
	int oldValue = this->getVerticesCount();

	if (oldValue == value || value < 2)
		return;

	for (int row = 0; row < std::max(oldValue, value); row++) {
		if (row < std::min(oldValue, value)) {
			for (int column = std::min(oldValue, value); column < std::max(oldValue, value); column++) {
				if (oldValue < value)
					this->adjacencyMatrix.at(row).push_back(0);
				else
					this->adjacencyMatrix.at(row).pop_back();
			}
		}
		else if (oldValue < value) {
			this->adjacencyMatrix.push_back(std::vector(value, 0));
			this->vertices.push_back(Vertex());
		}
		else {
			this->adjacencyMatrix.pop_back();
			this->vertices.pop_back();
		}
	}
}

int Graph::getEdge(int vertex1, int vertex2) const
{
	if (vertex1 < 0 || vertex1 >= this->getVerticesCount() || vertex2 < 0 || vertex2 >= this->getVerticesCount())
		return 0;

	return this->adjacencyMatrix.at(vertex1).at(vertex2);
}

std::vector<std::vector<int>>& Graph::getAdjacencyMatrix()
{
	return this->adjacencyMatrix;
}

std::vector<Vertex>& Graph::getVertices()
{
	return this->vertices;
}
