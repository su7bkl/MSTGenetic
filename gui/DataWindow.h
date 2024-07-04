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

        bool importGraph(std::string path);
        bool exportGraph(std::string path);

        void renderRandomGenerationButton();
        void renderAdjacencyMatrixTable();
        void renderGraphPreview();
        void renderModalWindows();

    public:
        DataWindow(genetic::Graph& graph, GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
