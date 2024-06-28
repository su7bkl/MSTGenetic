#pragma once
#include <vector>
#include <unordered_set>
#include <stack>
#include <cstdlib>
#include <stdexcept>

#define MAX_X 500
#define MAX_Y 500


namespace matrix {
	typedef std::pair<int, int> Coords;
	typedef std::pair<int, int> Terminals;
	typedef std::pair<Terminals, int> Edge;

	class Vertex {
	private:
		Coords pos;
	public:
		Vertex();
		Vertex(int x, int y);
		Coords getPos();
		int getX();
		int getY();
		void setPos(Coords newPos);
		void setX(int newX);
		void setY(int newX);
	};

	class Graph {
	private:
		int vertexCount;
		int edgeCount;
		std::vector<Vertex> vertices;
		std::vector<std::vector<int>> ajacencyMatrix;
	public:
		Graph();
		Graph(std::vector<Vertex> vertices, std::vector<Edge>edges );
		void addVertex(Vertex vertex);
		void addEdge(Edge);
		int getVeretexCount();
		int getEdgeCount();
		std::vector<Vertex> getVertices();
		Vertex getVertex(int id);
		int getEgdeLength(Terminals terminals);
		void changeEgdeLength(Terminals terminals, int newLength);
		void removeLastVertex();
		bool isConnected();
	};
}