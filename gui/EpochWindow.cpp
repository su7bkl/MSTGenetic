#include "gui/EpochWindow.h"
#include <imgui.h>
#include <algorithm>
#include <functional>
#include <string>
#include <format>

EpochWindow::EpochWindow(Graph& graph) : graph(graph), chromosomesSortingRequired(false)
{
    for (int i = 0; i < 20; i++) {
        this->chromosomes.push_back(Chromosome(this->graph));
        this->chromosomesOrder.push_back(i);
    }

    for (int startVertexIndex = 0; startVertexIndex < this->graph.getVerticesCount(); startVertexIndex++)
        for (int endVertexIndex = startVertexIndex + 1; endVertexIndex < this->graph.getVerticesCount(); endVertexIndex++)
            if (this->graph.getEdge(startVertexIndex, endVertexIndex) != 0)
                this->edges.push_back(std::make_pair(startVertexIndex, endVertexIndex));
}

void EpochWindow::regenerateChromosomes()
{
    this->chromosomes.clear();
    this->chromosomesOrder.clear();

    for (int i = 0; i < 20; i++) {
        this->chromosomes.push_back(Chromosome(this->graph));
        this->chromosomesOrder.push_back(i);
    }

    this->edges.clear();
    for (int startVertexIndex = 0; startVertexIndex < this->graph.getVerticesCount(); startVertexIndex++)
        for (int endVertexIndex = startVertexIndex + 1; endVertexIndex < this->graph.getVerticesCount(); endVertexIndex++)
            if (this->graph.getEdge(startVertexIndex, endVertexIndex) != 0)
                this->edges.push_back(std::make_pair(startVertexIndex, endVertexIndex));

    this->chromosomesSortingRequired = true;
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

    // сравнение в зависимости от целевого столбца
    switch (sortSpecs->Specs[0].ColumnIndex) {
    case COLUMN_ID:
        comparisionResult = left < right;
        break;
    case COLUMN_FUNCTION_VALUE:
        comparisionResult = this->chromosomes[left].getTargetFunctionValue() < this->chromosomes[right].getTargetFunctionValue();
        break;
    }

    // инвертирование результата для сортировки по убыванию
    if (sortSpecs->Specs[0].SortDirection == ImGuiSortDirection_Descending)
        comparisionResult = !comparisionResult;

    return comparisionResult;
}

void EpochWindow::render()
{
    // окно
    if (!ImGui::Begin((const char*)u8"Информация об эпохе"))
        return ImGui::End();

    static int selectedChromosome = -1;

    // кнопка "до конца"
    if (ImGui::Button((const char*)u8"До конца")) {
        // пока ничего
    }

    ImGui::SameLine();

    // кнопка "эпоха вперёд"
    if (ImGui::Button((const char*)u8"Эпоха вперёд")) {
        selectedChromosome = -1;
        this->regenerateChromosomes();
    }

    ImGui::SameLine();

    // кнопка "эпоха назад"
    if (ImGui::Button((const char*)u8"Эпоха назад")) {
        // пока ничего
    }

    ImGui::SameLine();

    // кнопка "остановить"
    if (ImGui::Button((const char*)u8"Остановить")) {
        // пока ничего
    }

    // таблица хромосом эпохи
    constexpr ImGuiTableFlags CHROMOSOMES_TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable;
    constexpr ImGuiTableColumnFlags CHROMOSOMES_COLUMN_FLAGS = ImGuiTableColumnFlags_None;
    constexpr ImGuiSelectableFlags ROW_SELECTABLE_FLAGS = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;

    if (ImGui::BeginTable("##chromosomes_table", 2, CHROMOSOMES_TABLE_FLAGS, ImVec2(0, 200))) {
        ImGui::TableSetupColumn((const char*)u8"ID", CHROMOSOMES_COLUMN_FLAGS);
        ImGui::TableSetupColumn((const char*)u8"Значение целевой функции", CHROMOSOMES_COLUMN_FLAGS);

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        this->sortChromosomes(ImGui::TableGetSortSpecs());

        for (const auto& row : this->chromosomesOrder) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Selectable(std::to_string(row).c_str(), row == selectedChromosome, ROW_SELECTABLE_FLAGS))
                selectedChromosome = row == selectedChromosome ? -1 : row;

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.3f", this->chromosomes.at(row).getTargetFunctionValue());
        }
        ImGui::EndTable();
    }

    // если хромосома не выбрана, не отрисовывать ничего дальше
    if (selectedChromosome == -1)
        return ImGui::End();

    ImGui::SeparatorText((const char*)u8"Информация о выбранной хромосоме");

    ImGui::Text((const char*)u8"Геном");

    // таблица генома (ребро - вкл/искл)
    constexpr ImGuiTableFlags GENOME_TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit;
    constexpr ImGuiTableColumnFlags GENOME_COLUMN_FLAGS = ImGuiTableColumnFlags_None;

    if (ImGui::BeginTable("##genome_table", this->edges.size() + 1, GENOME_TABLE_FLAGS, ImVec2(0, 50))) {
        ImGui::TableSetupColumn((const char*)u8"Ребро", GENOME_COLUMN_FLAGS);
        for (const auto& edge : this->edges)
            ImGui::TableSetupColumn(std::format("{},{}", edge.first, edge.second).c_str(), GENOME_COLUMN_FLAGS);

        ImGui::TableSetupScrollFreeze(1, 0);

        ImGui::TableHeadersRow();
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text((const char*)u8"Включено");

        for (int edgeIndex = 0; edgeIndex < this->edges.size(); edgeIndex++) {
            ImGui::TableSetColumnIndex(edgeIndex + 1);
            ImGui::Text(this->chromosomes.at(selectedChromosome).getGenome().at(edgeIndex) ? "1" : "0");
        }

        ImGui::EndTable();
    }

    ImGui::Spacing();

    // информация о связности подграфа
    ImGui::AlignTextToFramePadding();
    ImGui::Text((const char*)u8"Связность графа:");
    ImGui::SameLine();
    ImGui::BeginDisabled();
    bool isConnected = this->chromosomes.at(selectedChromosome).getIsConnected();
    ImGui::Checkbox("##", &isConnected);
    ImGui::EndDisabled();

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
        for (int edgeIndex = 0; edgeIndex < this->edges.size(); edgeIndex++) {
            Vertex startVertex = this->graph.getVertices().at(this->edges.at(edgeIndex).first);
            Vertex endVertex = this->graph.getVertices().at(this->edges.at(edgeIndex).second);
            bool isEdgeIncluded = this->chromosomes.at(selectedChromosome).getGenome().at(edgeIndex);

            drawList->AddLine(ImVec2(startVertex.x + drawOrigin.x, startVertex.y + drawOrigin.y), ImVec2(endVertex.x + drawOrigin.x, endVertex.y + drawOrigin.y), isEdgeIncluded ? EDGE_INCLUDED_COLOR : EDGE_COLOR, EDGE_THICKNESS);
        }

        // отрисовка вершин
        for (int vertexIndex = 0; vertexIndex < this->graph.getVerticesCount(); vertexIndex++) {
            Vertex currentVertex = this->graph.getVertices().at(vertexIndex);

            drawList->AddCircleFilled(ImVec2(currentVertex.x + drawOrigin.x, currentVertex.y + drawOrigin.y), VERTEX_SIZE, VERTEX_COLOR);
            drawList->AddText(ImVec2(currentVertex.x - VERTEX_SIZE + VERTEX_LABEL_OFFSET.x + drawOrigin.x, currentVertex.y - VERTEX_SIZE + VERTEX_LABEL_OFFSET.y + drawOrigin.y), VERTEX_LABEL_COLOR, std::to_string(vertexIndex).c_str());
        }

        // заглушка для размера
        ImGui::Dummy(ImVec2(500, 500));
    }
    ImGui::EndChild();

    ImGui::End();
}
