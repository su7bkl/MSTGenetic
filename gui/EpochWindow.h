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

        void sortChromosomes(ImGuiTableSortSpecs* sortSpecs);
        bool chromosomesComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const;

    public:
        EpochWindow(genetic::Graph& graph, GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
