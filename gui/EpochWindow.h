#pragma once
#include "gui/BaseWindow.h"
#include <vector>
#include <imgui.h>
#include "logic/Graph.h"
#include "logic/Genetic.h"

namespace gui {
    class EpochWindow : public BaseWindow
    {
    private:
        genetic::Graph& graph;
        genetic::GeneticAlgorithm& geneticAlgorithm;

        std::vector<int> chromosomesOrder;
        bool chromosomesSortingRequired;
        int selectedChromosome;

        std::vector<genetic::Terminals> graphEdges;

        void sortChromosomes(ImGuiTableSortSpecs* sortSpecs);
        bool chromosomesComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const;

        void renderChromosomesTable();
        void renderChromosomeInformation();

        void drawDashLine(ImDrawList* drawList, ImVec2 start, ImVec2 end, ImU32 color, float thickness, float interval);
        void renderGraphPreview();

    public:
        EpochWindow(genetic::Graph& graph, genetic::GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
