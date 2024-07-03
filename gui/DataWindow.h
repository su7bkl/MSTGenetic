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
        void ensureEdgeLength(int vertex1, int vertex2, int length);

        void renderRandomGenerationButton();
        void renderAdjacencyMatrixTable();
        void renderGraphPreview();

    public:
        DataWindow(genetic::Graph& graph, GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
