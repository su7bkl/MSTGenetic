#pragma once
#include "gui/BaseWindow.h"
#include "logic/Graph.h"
#include "logic/Genetic.h"

namespace gui {
    class DataWindow : public BaseWindow
    {
    private:
        genetic::Graph& graph;
        genetic::GeneticAlgorithm& geneticAlgorithm;

        void ensureVertexCount(int vertexCount);
        void ensureEdgeLength(int vertex1, int vertex2, int length);

        bool importGraph(std::string path);
        bool exportGraph(std::string path);

        void renderRandomGenerationButton();
        void renderAdjacencyMatrixTable();
        void renderGraphPreview();
        void renderModalWindows();

    public:
        DataWindow(genetic::Graph& graph, genetic::GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
