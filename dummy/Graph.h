#pragma once
#include <vector>
#include "Vertex.h"

class Graph
{
private:
    std::vector<Vertex> vertices;
    std::vector<std::vector<int>> adjacencyMatrix;

public:
    Graph(int vertices);

    int getVerticesCount() const;
    void setVerticesCount(int value);
    int getEdge(int vertex1, int vertex2) const;

    std::vector<std::vector<int>>& getAdjacencyMatrix();
    std::vector<Vertex>& getVertices();
};
