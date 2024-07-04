#include "gui/EpochWindow.h"
#include <imgui.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <format>

namespace gui {
    EpochWindow::EpochWindow(genetic::Graph& graph, genetic::GeneticAlgorithm& geneticAlgorithm) :
        graph(graph),
        geneticAlgorithm(geneticAlgorithm),
        chromosomesSortingRequired(false),
        selectedChromosome(-1)
    {
    }

    void EpochWindow::sortChromosomes(ImGuiTableSortSpecs* sortSpecs)
    {
        // проверка на то, нужно ли что-то сортировать
        if (sortSpecs == nullptr || (!sortSpecs->SpecsDirty && !this->chromosomesSortingRequired))
            return;

        // сама сортировка
        std::sort(this->chromosomesOrder.begin(), this->chromosomesOrder.end(), std::bind(&EpochWindow::chromosomesComparator, this, sortSpecs, std::placeholders::_1, std::placeholders::_2));

        // сброс флагов
        sortSpecs->SpecsDirty = false;
        this->chromosomesSortingRequired = false;
    }

    bool EpochWindow::chromosomesComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const
    {
        bool comparisionResult = false;

        constexpr int COLUMN_ID = 0;
        constexpr int COLUMN_FUNCTION_VALUE = 1;
        constexpr int COLUMN_EDGES_LENGTH = 2;

        // целевые значения, которые будут сравниваться
        double targetLeftValue = left;
        double targetRightValue = right;

        // сравнение в зависимости от целевого столбца
        switch (sortSpecs->Specs[0].ColumnIndex) {
        case COLUMN_ID:
            targetLeftValue = left;
            targetRightValue = right;
            break;
        case COLUMN_FUNCTION_VALUE:
            targetLeftValue = this->geneticAlgorithm.getCurrentGeneration()->getEntity(left).second;
            targetRightValue = this->geneticAlgorithm.getCurrentGeneration()->getEntity(right).second;
            break;
        case COLUMN_EDGES_LENGTH:
            targetLeftValue = this->geneticAlgorithm.getCurrentGeneration()->getEntity(left).first->getGraph()->getTotalEdgeLength();
            targetRightValue = this->geneticAlgorithm.getCurrentGeneration()->getEntity(right).first->getGraph()->getTotalEdgeLength();
            break;
        }

        comparisionResult = targetLeftValue < targetRightValue;

        // строгий порядок антирефлексивен
        if (targetLeftValue == targetRightValue)
            return false;

        // инвертирование результата для сортировки по убыванию
        if (sortSpecs->Specs[0].SortDirection == ImGuiSortDirection_Descending)
            comparisionResult = !comparisionResult;

        return comparisionResult;
    }

    void EpochWindow::renderChromosomesTable()
    {
        // таблица хромосом эпохи
        constexpr ImGuiTableFlags CHROMOSOMES_TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable;
        constexpr ImGuiTableColumnFlags CHROMOSOMES_COLUMN_FLAGS = ImGuiTableColumnFlags_None;
        constexpr ImGuiSelectableFlags ROW_SELECTABLE_FLAGS = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;

        if (ImGui::BeginTable("##chromosomes_table", 3, CHROMOSOMES_TABLE_FLAGS, ImVec2(0, 200))) {
            ImGui::TableSetupColumn((const char*)u8"ID", CHROMOSOMES_COLUMN_FLAGS);
            ImGui::TableSetupColumn((const char*)u8"Значение целевой функции", CHROMOSOMES_COLUMN_FLAGS);
            ImGui::TableSetupColumn((const char*)u8"Сумма весов рёбер подграфа", CHROMOSOMES_COLUMN_FLAGS);

            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            this->sortChromosomes(ImGui::TableGetSortSpecs());

            for (const auto& row : this->chromosomesOrder) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Selectable(std::to_string(row).c_str(), row == this->selectedChromosome, ROW_SELECTABLE_FLAGS))
                    this->selectedChromosome = row == this->selectedChromosome ? -1 : row;

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.6f", this->geneticAlgorithm.getCurrentGeneration()->getEntity(row).second);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", this->geneticAlgorithm.getCurrentGeneration()->getEntity(row).first->getGraph()->getTotalEdgeLength());
            }
            ImGui::EndTable();
        }
    }

    void EpochWindow::renderChromosomeInformation()
    {
        ImGui::SeparatorText((const char*)u8"Информация о выбранной хромосоме");

        ImGui::Text((const char*)u8"Геном");

        // таблица генома (ребро - вкл/искл)
        constexpr ImGuiTableFlags GENOME_TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit;
        constexpr ImGuiTableColumnFlags GENOME_COLUMN_FLAGS = ImGuiTableColumnFlags_None;

        if (ImGui::BeginTable("##genome_table", this->graphEdges.size() + 1, GENOME_TABLE_FLAGS, ImVec2(0, 50))) {
            ImGui::TableSetupColumn((const char*)u8"Ребро", GENOME_COLUMN_FLAGS);
            for (const auto& edge : this->graphEdges)
                ImGui::TableSetupColumn(std::format("{},{}", edge.first, edge.second).c_str(), GENOME_COLUMN_FLAGS);

            ImGui::TableSetupScrollFreeze(1, 0);

            ImGui::TableHeadersRow();
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text((const char*)u8"Включено");

            for (int edgeIndex = 0; edgeIndex < this->graphEdges.size(); edgeIndex++) {
                ImGui::TableSetColumnIndex(edgeIndex + 1);
                ImGui::Text(this->geneticAlgorithm.getCurrentGeneration()->getEntity(this->selectedChromosome).first->getIncluded().at(edgeIndex) ? "1" : "0");
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();

        // информация о связности подграфа
        ImGui::AlignTextToFramePadding();
        ImGui::Text((const char*)u8"Связность графа:");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        bool isConnected = this->geneticAlgorithm.getCurrentGeneration()->getEntity(this->selectedChromosome).first->getGraph()->isConnected();
        ImGui::Checkbox("##", &isConnected);
        ImGui::EndDisabled();

        // отрисовка подграфа
        this->renderGraphPreview();
    }

    void EpochWindow::renderGraphPreview()
    {
        ImGui::Text((const char*)u8"Представление подграфа:");

        // отрисовка подграфа
        constexpr ImGuiChildFlags GRAPH_RENDER_CHILD_FLAGS = ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX;
        constexpr ImGuiWindowFlags GRAPH_RENDER_WINDOW_FLAGS = ImGuiWindowFlags_HorizontalScrollbar;

        if (ImGui::BeginChild("##graph_render", ImVec2(-FLT_MIN, -FLT_MIN), GRAPH_RENDER_CHILD_FLAGS, GRAPH_RENDER_WINDOW_FLAGS)) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

            // настрока отрисовки (можно вынести в отдельное окно)
            constexpr ImU32 EDGE_COLOR = IM_COL32(255, 255, 255, 255);
            constexpr ImU32 EDGE_INCLUDED_COLOR = IM_COL32(0, 255, 0, 255);
            constexpr float EDGE_THICKNESS = 3;

            constexpr ImU32 VERTEX_COLOR = IM_COL32(255, 255, 255, 255);
            constexpr ImU32 VERTEX_LABEL_COLOR = IM_COL32(0, 0, 0, 255);
            constexpr ImVec2 VERTEX_LABEL_OFFSET = ImVec2(5, 5);
            constexpr float VERTEX_SIZE = 10;

            // отрисовка рёбер
            for (int edgeIndex = 0; edgeIndex < this->graphEdges.size(); edgeIndex++) {
                genetic::Vertex startVertex = this->graph.getVertex(this->graphEdges.at(edgeIndex).first);
                genetic::Vertex endVertex = this->graph.getVertex(this->graphEdges.at(edgeIndex).second);

                bool isEdgeIncluded = this->geneticAlgorithm.getCurrentGeneration()->getEntity(this->selectedChromosome).first->getIncluded().at(edgeIndex);

                drawList->AddLine(ImVec2(startVertex.getX() + drawOrigin.x, startVertex.getY() + drawOrigin.y), ImVec2(endVertex.getX() + drawOrigin.x, endVertex.getY() + drawOrigin.y), isEdgeIncluded ? EDGE_INCLUDED_COLOR : EDGE_COLOR, EDGE_THICKNESS);
            }

            // отрисовка вершин
            for (int vertexIndex = 0; vertexIndex < this->graph.getVeretexCount(); vertexIndex++) {
                genetic::Vertex currentVertex = this->graph.getVertex(vertexIndex);

                drawList->AddCircleFilled(ImVec2(currentVertex.getX() + drawOrigin.x, currentVertex.getY() + drawOrigin.y), VERTEX_SIZE, VERTEX_COLOR);
                drawList->AddText(ImVec2(currentVertex.getX() - VERTEX_SIZE + VERTEX_LABEL_OFFSET.x + drawOrigin.x, currentVertex.getY() - VERTEX_SIZE + VERTEX_LABEL_OFFSET.y + drawOrigin.y), VERTEX_LABEL_COLOR, std::to_string(vertexIndex).c_str());
            }

            // заглушка для размера
            ImGui::Dummy(ImVec2(500, 500));
        }
        ImGui::EndChild();
    }

    void EpochWindow::render()
    {
        // окно
        if (!ImGui::Begin((const char*)u8"Информация об эпохе"))
            return ImGui::End();

        // кнопка "запустить/остановить"
        {
            if (!this->graph.isConnected())
                ImGui::BeginDisabled();

            if (!this->geneticAlgorithm.isStarted() && ImGui::Button((const char*)u8"Запустить")) {
                this->geneticAlgorithm.start();
                this->selectedChromosome = -1;
                this->chromosomesSortingRequired = true;

                this->chromosomesOrder.resize(this->geneticAlgorithm.getGenerationSize());
                for (int i = 0; i < this->geneticAlgorithm.getGenerationSize(); i++)
                    this->chromosomesOrder.at(i) = i;

                this->graphEdges.clear();
                for (int startVertexIndex = 0; startVertexIndex < this->graph.getVeretexCount(); startVertexIndex++) {
                    for (int endVertexIndex = 0; endVertexIndex < startVertexIndex; endVertexIndex++) {
                        if (this->graph.getEdgeLength({ startVertexIndex, endVertexIndex }) == 0)
                            continue;
                        this->graphEdges.push_back({ startVertexIndex, endVertexIndex });
                    }
                }
            }

            else if (this->geneticAlgorithm.isStarted() && ImGui::Button((const char*)u8"Остановить")) {
                this->geneticAlgorithm.stop();
            }

            if (!this->graph.isConnected())
                ImGui::EndDisabled();
        }

        // если алгоритм не запущен, то не рисовать ничего, кроме кнопки запуска
        if (!this->geneticAlgorithm.isStarted())
            return ImGui::End();

        ImGui::SameLine();

        // кнопка "до конца"
        if (ImGui::Button((const char*)u8"До конца")) {
            // пока ничего
        }

        ImGui::SameLine();

        // кнопка "эпоха вперёд"
        if (ImGui::Button((const char*)u8"Эпоха вперёд")) {
            this->geneticAlgorithm.stepForward();
            this->chromosomesSortingRequired = true;
        }

        ImGui::SameLine();

        // кнопка "эпоха назад"
        if (ImGui::Button((const char*)u8"Эпоха назад")) {
            this->geneticAlgorithm.stepBack();
            this->chromosomesSortingRequired = true;
        }

        // таблица хромосом
        this->renderChromosomesTable();

        // если хромосома не выбрана, дальше ничего не отрисовывать  
        if (this->selectedChromosome == -1)
            return ImGui::End();

        // отрисовка информации о выбранной хромосоме
        this->renderChromosomeInformation();

        ImGui::End();
    }
}
