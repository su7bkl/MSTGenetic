#pragma once
#include "gui/BaseWindow.h"
#include "dummy/Graph.h"
#include "dummy/Chromosome.h"
#include <vector>
#include <imgui.h>

namespace gui {
    class EpochWindow : public BaseWindow
    {
    private:
        // будет убрано
        void regenerateChromosomes();
        std::vector<Chromosome> chromosomes;
        std::vector<std::pair<int, int>> edges;

        Graph& graph;

        std::vector<int> chromosomesOrder;
        bool chromosomesSortingRequired;

        void sortChromosomes(ImGuiTableSortSpecs* sortSpecs);
        bool chromosomesComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const;

    public:
        EpochWindow(Graph& graph);
        void render() override;
    };
}
