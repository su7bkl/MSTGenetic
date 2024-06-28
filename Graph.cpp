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

	void Vertex::setPos(Coords newPos) {
		this->pos = newPos;
	}

	void Vertex::setX(int newX) {
		this->pos.first = newX;
	}

	void Vertex::setY(int newY) {
		this->pos.second = newY;
	}
	
	/**
	* ����������� ������� �����
	*/
	Graph::Graph() {
		this->vertexCount = 0;
		this->edgeCount = 0;
	}

	/**
	* ����������� ����� �� ������� ������ vertices � ������� 
	* ���� edges
	* ��� ��� ���� ����������������� � �� �������� ������������, 
	* � edges �� ������ ����������� 2-� ���� � ����������� 
	* ��������� � �������� �������.
	* ���� ���� �� ���������� ����� �������� id ������� 
	* ������� ���� ��� ������� ��� ������ ���������� ���� � �����
	* ������������� ����������.
	*/
	Graph::Graph(std::vector<Vertex> vertices, std::vector<Edge> edges) {
		this->edgeCount = edges.size();
		this->vertexCount = vertices.size();
		this->vertices = vertices;
		this->ajacencyMatrix.resize(vertices.size());
		for (int i = 0; i < vertices.size(); i++) {
			this->ajacencyMatrix[i].resize(vertices.size());
		}
		for (Edge i : edges) {
			if (i.first.first < 0 || i.first.first >= this->vertexCount
				|| i.first.second < 0 || i.first.second >= this->vertexCount) {
				throw std::out_of_range("Vertex id out of range");
			}
			if (i.first.first == i.first.second) {
				throw std::invalid_argument("Attempt to add loop edge");
			}
			ajacencyMatrix[i.first.first][i.first.second] = i.second;
			ajacencyMatrix[i.first.second][i.first.first] = i.second;
		}
	}

	void Graph::addVertex(Vertex vertex) {
		this->vertexCount++;
		this->vertices.push_back(vertex);
	}

	void Graph::addEdge(Edge edge) {
		if (edge.first.first < 0 || edge.first.first >= this->vertexCount
			|| edge.first.second < 0 || edge.first.second >= this->vertexCount) {
			throw std::out_of_range("Vertex id out of range");
		}
		if (edge.first.first == edge.first.second) {
			throw std::invalid_argument("Attempt to add loop edge");
		}
		this->edgeCount++;
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

	/**
	* ���������� ����� ����� ����� ����� ���������
	* ���� ����� ��� ���������� -1.
	* ���� ��������� ������� ���������� � �����
	* ������� ���������� out_of_range
	*/
	int Graph::getEgdeLength(Terminals terminals) {
		if (terminals.first < 0 || terminals.first >= this->vertexCount
			|| terminals.second < 0 || terminals.second >= this->vertexCount) {
			throw std::out_of_range("Vertex id out of range");
		}
		if (this->ajacencyMatrix[terminals.first][terminals.second] == 0) {
			return -1;
		}
		return this->ajacencyMatrix[terminals.first][terminals.second];
	}
	
	/**
	* ������ ����� ����� ����� ����� ���������
	* ���� ����� ��� ������� ���������� invalid_argument.
	* ���� ��������� ������� ���������� � �����
	* ������� ���������� out_of_range
	*/
	void Graph::changeEgdeLength(Terminals terminals, int newLength) {
		if (terminals.first < 0 || terminals.first >= this->vertexCount
			|| terminals.second < 0 || terminals.second >= this->vertexCount) {
			throw std::out_of_range("Vertex id out of range");
		}
		if (this->ajacencyMatrix[terminals.first][terminals.second] == 0) {
			throw std::invalid_argument("Attempt to change length of non-existent edge");
		}
		this->ajacencyMatrix[terminals.first][terminals.second] = newLength;
		this->ajacencyMatrix[terminals.second][terminals.first] = newLength;
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

	void Graph::removeLastVertex() {
		this->vertexCount--;
		int removedEdges = 0;
		for (int i = 0; i < vertexCount; i++) {
			this->ajacencyMatrix[i].pop_back();
			if (this->ajacencyMatrix[this->vertexCount][i] != 0) {
				removedEdges++;
			}
		}
		this->ajacencyMatrix.pop_back();
		this->edgeCount -= removedEdges;
		this->vertices.pop_back();
	}
}