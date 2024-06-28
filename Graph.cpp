#include "Graph.h"


namespace matrix {
	Vertex::Vertex() {
		this->pos = { std::rand() % MAX_X, std::rand() % MAX_Y };
	}

	Vertex::Vertex(int x, int y) {
		this->pos = { x, y };
	}

	Coords Vertex::getPos() {
		return this->pos;
	}

	int Vertex::getX() {
		return this->pos.first;
	}

	int Vertex::getY() {
		return this->pos.second;
	}

	Graph::Graph() {
		this->vertexCount = 0;
		this->edgeCount = 0;
	}

	Graph::Graph(std::vector<Vertex> vertices, std::vector<Edge> edges) {
		this->edgeCount = edges.size();
		this->vertexCount = vertices.size();
		this->vertices = vertices;
		this->ajacencyMatrix.resize(vertices.size());
		for (int i = 0; i < vertices.size(); i++) {
			this->ajacencyMatrix[i].resize(vertices.size());
		}
		for (Edge i : edges) {
			ajacencyMatrix[i.first.first][i.first.second] = i.second;
			ajacencyMatrix[i.first.second][i.first.first] = i.second;
		}
	}

	void Graph::addVertex(Vertex vertex) {
		this->vertexCount++;
		this->vertices.push_back(vertex);
	}

	void Graph::addEdge(Edge edge) {
		this->ajacencyMatrix[edge.first.first][edge.first.second] = edge.second;
		this->ajacencyMatrix[edge.first.second][edge.first.first] = edge.second;
	}

	int Graph::getVeretexCount() {
		return this->vertexCount;
	}

	int Graph::getEdgeCount() {
		return this->edgeCount;
	}

	std::vector<Vertex> Graph::getVertices() {
		return this->vertices;
	}

	Vertex Graph::getVertex(int id) {
		if (id < 0 || id >= this->vertexCount) {
			throw std::out_of_range("Vertex id out of range");
		}
		return this->vertices[id];
	}

	int Graph::getEgdeLength(Terminals terminals) {
		return this->ajacencyMatrix[terminals.first][terminals.second];
	}

	bool Graph::isConnected() {
		std::unordered_set<int> visited;
		std::stack<int> stack;
		int counter = 0;
		for (int i = 0; i < this->vertexCount; i++) {
			if (visited.find(i) == visited.end()) {
				counter++;
				stack.push(i);
				visited.insert(i);
				while (!stack.empty())
				{
					int current = stack.top();
					stack.pop();
					for (int j = 0; j < this->vertexCount; j++) {
						if (this->ajacencyMatrix[current][j] != 0 && visited.find(j) == visited.end()) {
							stack.push(j);
							visited.insert(j);
						}
					}
				}
			}
		}
		return counter == 1;
	}
}