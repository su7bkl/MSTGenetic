#pragma once
#include <vector>
#include <unordered_set>
#include <stack>
#include <cstdlib>
#include <stdexcept>
#include <numeric>

#define MAX_X 500
#define MAX_Y 500


namespace genetic {
	typedef std::pair<float, float> Coords;
	typedef std::pair<int, int> Terminals;
	typedef std::pair<Terminals, int> Edge;

	class Vertex {
	private:
		Coords pos;
	public:
		Vertex();
		Vertex(float xy);
		Vertex(float x, float y);
		Coords getPos();
		float getX();
		float getY();
		void setPos(Coords newPos);
		void setX(float newX);
		void setY(float newX);
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
        void changeVertexCoords(int id, Coords newPos);
		int getEdgeLength(Terminals terminals);
		int getTotalEdgeLength();
		int getConnectedComponentsCount();
		int getUpperMSTEstiamate();
		void changeEdgeLength(Terminals terminals, int newLength);
		void removeLastVertex();
		bool isConnected();
	};
}
