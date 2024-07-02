#include "gui/DataWindow.h"
#include <imgui.h>
#include <ImGuiFileDialog.h>
#include <algorithm>
#include <vector>
#include <string>

namespace gui {
    void DataWindow::ensureVertexCount(int vertexCount)
    {
        int difference = vertexCount - this->graph.getVeretexCount();

        if (difference == 0)
            return;

        if (difference > 0) {
            for (int i = 0; i < difference; i++)
                this->graph.addVertex(genetic::Vertex());
        }
        else {
            for (int i = 0; i < -difference; i++)
                this->graph.removeLastVertex();
        }
    }

    void DataWindow::ensureEdge(int vertex1, int vertex2, int length)
    {
        if (this->graph.getEdgeLength({ vertex1, vertex2 }) == length)
            return;

        if (this->graph.getEdgeLength({ vertex1, vertex2 }) == 0)
            this->graph.addEdge({ { vertex1, vertex2 }, length });
        else
            this->graph.changeEdgeLength({ vertex1, vertex2 }, length);
    }

    DataWindow::DataWindow(genetic::Graph& graph, GeneticAlgorithm& geneticAlgorithm) : graph(graph), geneticAlgorithm(geneticAlgorithm) {}

    void DataWindow::render()
    {
        // окно
        if (ImGui::Begin((const char*)u8"Данные")) {

            // если алгоритм запущен, то предотвратить ввод
            if (this->geneticAlgorithm.isStarted())
                ImGui::BeginDisabled();

            // ввод числа вершин
            int verticesCount = this->graph.getVeretexCount();
            ImGui::InputInt((const char*)u8"Число вершин", &verticesCount, 1, 10);
            verticesCount = std::clamp(verticesCount, 2, 500);
            this->ensureVertexCount(verticesCount);

            // кнопка импорта из файла
            if (ImGui::Button((const char*)u8"Импорт из файла")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.flags = ImGuiFileDialogFlags_Modal;
                ImGuiFileDialog::Instance()->OpenDialog("##importFileDialog", (const char*)u8"Выберите файл матрицы (НЕ РАБОТАЕТ)", ".*", config);
            }

            ImGui::SameLine();

            // кнопка рандомной генерации
            if (ImGui::Button((const char*)u8"Рандомная генерация")) {
                // пока ничего
            }

            ImGui::SeparatorText((const char*)u8"Матрица смежности");

            // матрица смежности
            constexpr ImGuiTableFlags ADJACENCY_TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;
            constexpr ImGuiTableColumnFlags ADJACENCY_COLUMN_FLAGS = ImGuiTableColumnFlags_WidthFixed;

            verticesCount = this->graph.getVeretexCount();

            if (ImGui::BeginTable("##adjacency_matrix_table", verticesCount + 1, ADJACENCY_TABLE_FLAGS, ImVec2(0, 350))) {
                ImGui::TableSetupColumn("##", ADJACENCY_COLUMN_FLAGS);
                for (int column = 0; column < verticesCount; column++)
                    ImGui::TableSetupColumn(std::to_string(column).c_str(), ADJACENCY_COLUMN_FLAGS, 25);

                ImGui::TableSetupScrollFreeze(1, 1);
                ImGui::TableHeadersRow();
                for (int row = 0; row < verticesCount; row++) {
                    ImGui::PushID(row);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", row);
                    for (int column = 0; column < verticesCount; column++) {
                        if (ImGui::TableSetColumnIndex(column + 1)) {
                            ImGui::PushID(column + 1);
                            ImGui::SetNextItemWidth(-FLT_MIN);

                            if (row == column)
                                ImGui::Text("%d", this->graph.getEdgeLength({ row, column }));
                            else {
                                int edgeLength = this->graph.getEdgeLength({ row, column });
                                ImGui::InputInt("##", &edgeLength, 0);
                                edgeLength = std::max(edgeLength, 0);
                                this->ensureEdge(row, column, edgeLength);
                            }

                            ImGui::PopID();
                        }
                    }
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }

            if (this->geneticAlgorithm.isStarted())
                ImGui::EndDisabled();

            ImGui::SeparatorText((const char*)u8"Визуальное представление [?]");
            ImGui::SetItemTooltip((const char*)u8"Вершины графа можно перетаскивать");

            // отрисовка графа
            constexpr ImGuiChildFlags GRAPH_RENDER_CHILD_FLAGS = ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX;
            constexpr ImGuiWindowFlags GRAPH_RENDER_WINDOW_FLAGS = ImGuiWindowFlags_HorizontalScrollbar;

            if (ImGui::BeginChild("##graph_render", ImVec2(-FLT_MIN, -FLT_MIN), GRAPH_RENDER_CHILD_FLAGS, GRAPH_RENDER_WINDOW_FLAGS)) {
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

                // параметры отрисовки
                constexpr ImU32 EDGE_COLOR = IM_COL32(0, 255, 0, 255);
                constexpr float EDGE_THICKNESS = 3;

                constexpr ImU32 VERTEX_COLOR = IM_COL32(255, 255, 255, 255);
                constexpr ImU32 VERTEX_LABEL_COLOR = IM_COL32(0, 0, 0, 255);
                constexpr ImVec2 VERTEX_LABEL_OFFSET = ImVec2(5, 5);
                constexpr float VERTEX_SIZE = 10;

                // отрисовка рёбер
                for (int startVertexIndex = 0; startVertexIndex < verticesCount; startVertexIndex++) {
                    for (int endVertexIndex = startVertexIndex + 1; endVertexIndex < verticesCount; endVertexIndex++) {
                        if (this->graph.getEdgeLength({ startVertexIndex, endVertexIndex }) == 0)
                            continue;

                        genetic::Vertex startVertex = this->graph.getVertex(startVertexIndex);
                        genetic::Vertex endVertex = this->graph.getVertex(endVertexIndex);

                        drawList->AddLine(ImVec2(startVertex.getX() + drawOrigin.x, startVertex.getY() + drawOrigin.y), ImVec2(endVertex.getX() + drawOrigin.x, endVertex.getY() + drawOrigin.y), EDGE_COLOR, EDGE_THICKNESS);
                    }
                }

                // отрисовка вершин
                for (int vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
                    genetic::Vertex currentVertex = this->graph.getVertex(vertexIndex);

                    drawList->AddCircleFilled(ImVec2(currentVertex.getX() + drawOrigin.x, currentVertex.getY() + drawOrigin.y), VERTEX_SIZE, VERTEX_COLOR);
                    drawList->AddText(ImVec2(currentVertex.getX() - VERTEX_SIZE + VERTEX_LABEL_OFFSET.x + drawOrigin.x, currentVertex.getY() - VERTEX_SIZE + VERTEX_LABEL_OFFSET.y + drawOrigin.y), VERTEX_LABEL_COLOR, std::to_string(vertexIndex).c_str());
                }

                ImGuiIO& io = ImGui::GetIO();
                static int draggingVertexIndex = -1;

                // захват мыши для перетаскивания
                if (io.MouseClicked[ImGuiMouseButton_Left] && draggingVertexIndex == -1) {
                    for (int vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
                        genetic::Vertex currentVertex = this->graph.getVertices().at(vertexIndex);
                        float deltaX = drawOrigin.x + currentVertex.getX() - io.MousePos.x;
                        float deltaY = drawOrigin.y + currentVertex.getY() - io.MousePos.y;

                        if (deltaX * deltaX + deltaY * deltaY < VERTEX_SIZE * VERTEX_SIZE) {
                            draggingVertexIndex = vertexIndex;
                            break;
                        }
                    }
                }

                // перетаскивание
                if (draggingVertexIndex != -1) {
                    this->graph.changeVertexCoords(draggingVertexIndex, { io.MousePos.x - drawOrigin.x, io.MousePos.y - drawOrigin.y });

                    if (!io.MouseDown[ImGuiMouseButton_Left])
                        draggingVertexIndex = -1;
                }

                // заглушка для размера
                ImGui::Dummy(ImVec2(500, 500));
            }
            ImGui::EndChild();
        }
        ImGui::End();

        // диалоговое окно выбора файла
        if (ImGuiFileDialog::Instance()->Display("##importFileDialog")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filepath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // пока ничего не происходит
            }

            ImGuiFileDialog::Instance()->Close();
        }
    }
}
