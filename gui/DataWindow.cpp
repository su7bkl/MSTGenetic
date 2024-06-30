#include "gui/DataWindow.h"
#include <imgui.h>
#include <ImGuiFileDialog.h>
#include <algorithm>
#include <vector>
#include <string>

DataWindow::DataWindow(Graph& graph) : graph(graph) {}

void DataWindow::render()
{
    // окно
    if (ImGui::Begin((const char*)u8"Данные")) {

        // ввод числа вершин
        int verticesCount = this->graph.getVerticesCount();
        ImGui::InputInt((const char*)u8"Число вершин", &verticesCount, 1, 10);
        verticesCount = std::clamp(verticesCount, 2, 500);
        this->graph.setVerticesCount(verticesCount);

        // кнопка импорта из файла
        if (ImGui::Button((const char*)u8"Импорт из файла")) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            config.flags = ImGuiFileDialogFlags_Modal;
            ImGuiFileDialog::Instance()->OpenDialog("##importFileDialog", (const char*)u8"Выберите файл матрицы", ".*", config);
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

        verticesCount = this->graph.getVerticesCount();
        auto& adjacencyMatrix = this->graph.getAdjacencyMatrix();

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
                            ImGui::Text("%d", adjacencyMatrix.at(row).at(column));
                        else if (row < column) {
                            ImGui::InputInt("##", &adjacencyMatrix.at(row).at(column), 0);
                            adjacencyMatrix.at(row).at(column) = std::max(adjacencyMatrix.at(row).at(column), 0);
                        }
                        else {
                            ImGui::InputInt("##", &adjacencyMatrix.at(column).at(row), 0);
                            adjacencyMatrix.at(column).at(row) = std::max(adjacencyMatrix.at(column).at(row), 0);
                        }

                        ImGui::PopID();
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }

        ImGui::SeparatorText((const char*)u8"Визуальное представление");

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
                    if (adjacencyMatrix.at(startVertexIndex).at(endVertexIndex) == 0)
                        continue;

                    Vertex startVertex = this->graph.getVertices().at(startVertexIndex);
                    Vertex endVertex = this->graph.getVertices().at(endVertexIndex);

                    drawList->AddLine(ImVec2(startVertex.x + drawOrigin.x, startVertex.y + drawOrigin.y), ImVec2(endVertex.x + drawOrigin.x, endVertex.y + drawOrigin.y), EDGE_COLOR, EDGE_THICKNESS);
                }
            }

            // отрисовка вершин
            for (int vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
                Vertex currentVertex = this->graph.getVertices().at(vertexIndex);

                drawList->AddCircleFilled(ImVec2(currentVertex.x + drawOrigin.x, currentVertex.y + drawOrigin.y), VERTEX_SIZE, VERTEX_COLOR);
                drawList->AddText(ImVec2(currentVertex.x - VERTEX_SIZE + VERTEX_LABEL_OFFSET.x + drawOrigin.x, currentVertex.y - VERTEX_SIZE + VERTEX_LABEL_OFFSET.y + drawOrigin.y), VERTEX_LABEL_COLOR, std::to_string(vertexIndex).c_str());
            }

            ImGuiIO& io = ImGui::GetIO();
            static int draggingVertexIndex = -1;

            // захват мыши для перетаскивания
            if (io.MouseClicked[ImGuiMouseButton_Left] && draggingVertexIndex == -1) {
                for (int vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
                    Vertex currentVertex = this->graph.getVertices().at(vertexIndex);
                    float deltaX = drawOrigin.x + currentVertex.x - io.MousePos.x;
                    float deltaY = drawOrigin.y + currentVertex.y - io.MousePos.y;

                    if (deltaX * deltaX + deltaY * deltaY < VERTEX_SIZE * VERTEX_SIZE) {
                        draggingVertexIndex = vertexIndex;
                        break;
                    }
                }
            }

            // перетаскивание
            if (draggingVertexIndex != -1) {
                this->graph.getVertices().at(draggingVertexIndex).x = io.MousePos.x - drawOrigin.x;
                this->graph.getVertices().at(draggingVertexIndex).y = io.MousePos.y - drawOrigin.y;

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
        }

        ImGuiFileDialog::Instance()->Close();
    }
}
