#pragma once
#include "gui/BaseWindow.h"
#include "Graph.h"
#include "dummy/GeneticAlgorithm.h"

namespace gui {
    class DataWindow : public BaseWindow
    {
    private:
        genetic::Graph& graph;
        GeneticAlgorithm& geneticAlgorithm;

        void ensureVertexCount(int vertexCount);
        void ensureEdge(int vertex1, int vertex2, int length);

    public:
        DataWindow(genetic::Graph& graph, GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
