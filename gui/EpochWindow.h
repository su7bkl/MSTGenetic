#pragma once
#include "gui/BaseWindow.h"
#include "Graph.h"
#include <vector>
#include <imgui.h>
#include "dummy/GeneticAlgorithm.h"

namespace gui {
    class EpochWindow : public BaseWindow
    {
    private:
        genetic::Graph& graph;
        GeneticAlgorithm& geneticAlgorithm;

        std::vector<int> chromosomesOrder;
        bool chromosomesSortingRequired;
        int selectedChromosome;

        // будет убрано
        std::vector<genetic::Terminals> graphEdges;

        void sortChromosomes(ImGuiTableSortSpecs* sortSpecs);
        bool chromosomesComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const;

        void renderChromosomesTable();
        void renderChromosomeInformation();
        void renderGraphPreview();

    public:
        EpochWindow(genetic::Graph& graph, GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
