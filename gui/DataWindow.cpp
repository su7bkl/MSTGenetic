#include "gui/DataWindow.h"
#include <imgui.h>
#include <ImGuiFileDialog.h>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <random>

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

    void DataWindow::ensureEdgeLength(int vertex1, int vertex2, int length)
    {
        if (this->graph.getEdgeLength({ vertex1, vertex2 }) == length)
            return;

        if (this->graph.getEdgeLength({ vertex1, vertex2 }) == 0)
            this->graph.addEdge({ { vertex1, vertex2 }, length });
        else
            this->graph.changeEdgeLength({ vertex1, vertex2 }, length);
    }

    bool DataWindow::importGraph(std::string path)
    {
        // инициализация файла
        std::ifstream importFile(path);

        // временное хранение считанных данных для валидации
        int newVertexCount = 0;
        std::vector<std::vector<int>> newAdjacencyMatrix;
        std::vector<genetic::Coords> newVerexCoords;

        // считывание числа вершин
        importFile >> newVertexCount;
        if (importFile.fail() || newVertexCount < 2) {
            importFile.close();
            return false;
        }

        // считывание матрицы смежности
        newAdjacencyMatrix.resize(newVertexCount);
        for (int startVertexIndex = 0; startVertexIndex < newVertexCount; startVertexIndex++) {
            newAdjacencyMatrix.at(startVertexIndex).resize(newVertexCount);

            for (int endVertexIndex = 0; endVertexIndex < newVertexCount; endVertexIndex++) {
                importFile >> newAdjacencyMatrix.at(startVertexIndex).at(endVertexIndex);

                if (importFile.fail() || newAdjacencyMatrix.at(startVertexIndex).at(endVertexIndex) < 0) {
                    importFile.close();
                    return false;
                }

                if (startVertexIndex == endVertexIndex && newAdjacencyMatrix.at(startVertexIndex).at(endVertexIndex) != 0) {
                    importFile.close();
                    return false;
                }
            }
        }

        // считывание координат вершин
        newVerexCoords.resize(newVertexCount);
        for (int vertexIndex = 0; vertexIndex < newVertexCount; vertexIndex++) {
            importFile >> newVerexCoords.at(vertexIndex).first >> newVerexCoords.at(vertexIndex).second;

            if (importFile.fail()) {
                importFile.close();
                return false;
            }
        }

        importFile.close();

        // перенос считанных данных если всё нормально
        this->ensureVertexCount(newVertexCount);
        for (int startVertexIndex = 0; startVertexIndex < newVertexCount; startVertexIndex++) {
            for (int endVertexIndex = 0; endVertexIndex < newVertexCount; endVertexIndex++) {
                this->ensureEdgeLength(startVertexIndex, endVertexIndex, newAdjacencyMatrix.at(startVertexIndex).at(endVertexIndex));
            }

            this->graph.changeVertexCoords(startVertexIndex, newVerexCoords.at(startVertexIndex));
        }

        return true;
    }

    bool DataWindow::exportGraph(std::string path)
    {
        // открытие файла на запись
        std::ofstream exportFile(path, std::ofstream::trunc);

        // проверка на ошибки
        if (!exportFile.is_open())
            return false;

        // запись числа вершин
        exportFile << this->graph.getVeretexCount() << "\n";

        // запись матрицы смежности
        for (int startVertexIndex = 0; startVertexIndex < this->graph.getVeretexCount(); startVertexIndex++) {
            for (int endVertexIndex = 0; endVertexIndex < this->graph.getVeretexCount(); endVertexIndex++) {
                exportFile << this->graph.getEdgeLength({ startVertexIndex, endVertexIndex }) << " ";
            }

            exportFile << "\n";
        }

        // запись координат вершин
        for (int vertexIndex = 0; vertexIndex < this->graph.getVeretexCount(); vertexIndex++) {
            exportFile << this->graph.getVertex(vertexIndex).getX() << " " << this->graph.getVertex(vertexIndex).getY() << "\n";
        }

        exportFile.close();
        return true;
    }

    void DataWindow::renderRandomGenerationButton()
    {
        // параметры случайной генерации
        static int minEdgeLength = 1;
        static int maxEdgeLength = 10;
        static float edgeProbability = 0.5;

        // кнопка рандомной генерации
        if (ImGui::Button((const char*)u8"Рандомная генерация [?]")) {
            std::mt19937 rng((std::random_device())());
            std::bernoulli_distribution createEdge(edgeProbability);
            std::uniform_int_distribution<int> edgeLength(minEdgeLength, maxEdgeLength);

            for (int startVertexIndex = 0; startVertexIndex < this->graph.getVeretexCount(); startVertexIndex++) {
                for (int endVertexIndex = 0; endVertexIndex < startVertexIndex; endVertexIndex++) {
                    if (createEdge(rng))
                        this->ensureEdgeLength(startVertexIndex, endVertexIndex, edgeLength(rng));
                    else
                        this->ensureEdgeLength(startVertexIndex, endVertexIndex, 0);
                }
            }
        }

        // подсказка
        ImGui::SetItemTooltip((const char*)u8"ПКМ чтобы настроить рандомную генерацию");

        // настройки
        if (ImGui::BeginPopupContextItem((const char*)u8"Настройки генерации")) {
            ImGui::DragInt((const char*)u8"Мин. вес", &minEdgeLength, 1, 1, INT_MAX);

            maxEdgeLength = std::max(minEdgeLength, maxEdgeLength);
            ImGui::DragInt((const char*)u8"Макс. вес", &maxEdgeLength, 1, minEdgeLength, INT_MAX);

            ImGui::SliderFloat((const char*)u8"Заполненность", &edgeProbability, 0, 1);
            ImGui::EndPopup();
        }
    }

    void DataWindow::renderAdjacencyMatrixTable()
    {
        ImGui::SeparatorText((const char*)u8"Матрица смежности");

        // матрица смежности
        constexpr ImGuiTableFlags ADJACENCY_TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;
        constexpr ImGuiTableColumnFlags ADJACENCY_COLUMN_FLAGS = ImGuiTableColumnFlags_WidthFixed;

        const int vertexCount = this->graph.getVeretexCount();

        if (ImGui::BeginTable("##adjacency_matrix_table", vertexCount + 1, ADJACENCY_TABLE_FLAGS, ImVec2(0, 350))) {
            ImGui::TableSetupColumn("##", ADJACENCY_COLUMN_FLAGS);
            for (int column = 0; column < vertexCount; column++)
                ImGui::TableSetupColumn(std::to_string(column).c_str(), ADJACENCY_COLUMN_FLAGS, 25);

            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableHeadersRow();
            for (int row = 0; row < vertexCount; row++) {
                ImGui::PushID(row);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", row);
                for (int column = 0; column < vertexCount; column++) {
                    if (ImGui::TableSetColumnIndex(column + 1)) {
                        ImGui::PushID(column + 1);
                        ImGui::SetNextItemWidth(-FLT_MIN);

                        if (row == column)
                            ImGui::Text("%d", this->graph.getEdgeLength({ row, column }));
                        else {
                            int edgeLength = this->graph.getEdgeLength({ row, column });
                            ImGui::InputInt("##", &edgeLength, 0);
                            edgeLength = std::max(edgeLength, 0);
                            this->ensureEdgeLength(row, column, edgeLength);
                        }

                        ImGui::PopID();
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
    }

    void DataWindow::renderGraphPreview()
    {
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

            const int vertexCount = this->graph.getVeretexCount();

            // отрисовка рёбер
            for (int startVertexIndex = 0; startVertexIndex < vertexCount; startVertexIndex++) {
                for (int endVertexIndex = startVertexIndex + 1; endVertexIndex < vertexCount; endVertexIndex++) {
                    if (this->graph.getEdgeLength({ startVertexIndex, endVertexIndex }) == 0)
                        continue;

                    genetic::Vertex startVertex = this->graph.getVertex(startVertexIndex);
                    genetic::Vertex endVertex = this->graph.getVertex(endVertexIndex);

                    drawList->AddLine(ImVec2(startVertex.getX() + drawOrigin.x, startVertex.getY() + drawOrigin.y), ImVec2(endVertex.getX() + drawOrigin.x, endVertex.getY() + drawOrigin.y), EDGE_COLOR, EDGE_THICKNESS);
                }
            }

            // отрисовка вершин
            for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
                genetic::Vertex currentVertex = this->graph.getVertex(vertexIndex);

                drawList->AddCircleFilled(ImVec2(currentVertex.getX() + drawOrigin.x, currentVertex.getY() + drawOrigin.y), VERTEX_SIZE, VERTEX_COLOR);
                drawList->AddText(ImVec2(currentVertex.getX() - VERTEX_SIZE + VERTEX_LABEL_OFFSET.x + drawOrigin.x, currentVertex.getY() - VERTEX_SIZE + VERTEX_LABEL_OFFSET.y + drawOrigin.y), VERTEX_LABEL_COLOR, std::to_string(vertexIndex).c_str());
            }

            ImGuiIO& io = ImGui::GetIO();
            static int draggingVertexIndex = -1;

            // захват мыши для перетаскивания
            if (io.MouseClicked[ImGuiMouseButton_Left] && draggingVertexIndex == -1) {
                for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
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

    void DataWindow::renderModalWindows()
    {
        constexpr ImVec2 MIN_FILE_DIALOG_SIZE = ImVec2(400, 300);

        // диалоговое окно импорта графа
        if (ImGuiFileDialog::Instance()->Display("##importFileDialog", ImGuiWindowFlags_NoCollapse, MIN_FILE_DIALOG_SIZE)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filepath = ImGuiFileDialog::Instance()->GetCurrentPath();

                std::filesystem::path fullpath = filepath;
                fullpath /= filename;

                if (!this->importGraph(fullpath.generic_string()))
                    ImGui::OpenPopup((const char*)u8"Ошибка импорта");
            }

            ImGuiFileDialog::Instance()->Close();
        }

        // popup ошибки импорта
        if (ImGui::BeginPopupModal((const char*)u8"Ошибка импорта", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text((const char*)u8"Файл повреждён или имеет неподдерживаемый формат данных");
            if (ImGui::Button("OK"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        // диалоговое окно экспорта графа
        if (ImGuiFileDialog::Instance()->Display("##exportFileDialog", ImGuiWindowFlags_NoCollapse, MIN_FILE_DIALOG_SIZE)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filepath = ImGuiFileDialog::Instance()->GetCurrentPath();

                std::filesystem::path fullpath = filepath;
                fullpath /= filename;

                if (!this->exportGraph(fullpath.generic_string()))
                    ImGui::OpenPopup((const char*)u8"Ошибка экспорта!");
            }

            ImGuiFileDialog::Instance()->Close();
        }

        // popup ошибки экспорта
        if (ImGui::BeginPopupModal((const char*)u8"Ошибка экспорта!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text((const char*)u8"Невозможно открыть файл на запись");
            if (ImGui::Button("OK"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
    }

    DataWindow::DataWindow(genetic::Graph& graph, genetic::GeneticAlgorithm& geneticAlgorithm) :
        graph(graph),
        geneticAlgorithm(geneticAlgorithm)
    {}

    void DataWindow::render()
    {
        // окно
        if (ImGui::Begin((const char*)u8"Данные")) {

            // если алгоритм запущен, то предотвратить ввод
            if (this->geneticAlgorithm.isStarted())
                ImGui::BeginDisabled();

            // ввод числа вершин
            int vertexCount = this->graph.getVeretexCount();
            ImGui::InputInt((const char*)u8"Число вершин", &vertexCount, 1, 10);
            vertexCount = std::clamp(vertexCount, 2, 500);
            this->ensureVertexCount(vertexCount);

            // кнопка импорта из файла
            if (ImGui::Button((const char*)u8"Импорт из файла")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.flags = ImGuiFileDialogFlags_Modal;
                ImGuiFileDialog::Instance()->OpenDialog("##importFileDialog", (const char*)u8"Выберите файл матрицы", "Any file (*.*){.*}", config);
            }

            ImGui::SameLine();

            // кнопка экспорта в файл
            if (ImGui::Button((const char*)u8"Экспорт в файл")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
                ImGuiFileDialog::Instance()->OpenDialog("##exportFileDialog", (const char*)u8"Выберите куда экспортировать", "Any file (*.*){.*}", config);
            }

            ImGui::SameLine();

            // кнопка рандомной генерации
            this->renderRandomGenerationButton();

            // матрица смежности
            this->renderAdjacencyMatrixTable();

            if (this->geneticAlgorithm.isStarted())
                ImGui::EndDisabled();

            // отрисовка графа
            this->renderGraphPreview();
        }
        ImGui::End();

        // окна открытия файлов и ошибки открытия файлов
        this->renderModalWindows();
    }
}
