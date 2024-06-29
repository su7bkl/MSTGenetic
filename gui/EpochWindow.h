#pragma once
#include "gui/WindowBase.h"
#include "dummy/Graph.h"
#include "dummy/Chromosome.h"
#include <vector>
#include <imgui.h>

class EpochWindow : public WindowBase
{
private:
    Graph& graph;
    std::vector<Chromosome> chromosomes;
    std::vector<int> chromosomesOrder;
    bool chromosomesSortingRequired;
    std::vector<std::pair<int, int>> edges;
    int selectedChromosome;

    void regenerateChromosomes();
    void sortChromosomes(ImGuiTableSortSpecs* sortSpecs);
    bool sortComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const;

public:
    EpochWindow(Graph& graph);
    void render() override;
};
