#include "gui/DataWindow.h"
#include <imgui.h>
#include <algorithm>
#include <vector>
#include <string>

DataWindow::DataWindow(Graph& graph) : graph(graph)
{
}

void DataWindow::render()
{
	if (!ImGui::Begin((const char*)u8"Данные"))
		return ImGui::End();

	int verticesCount = this->graph.getVerticesCount();
	ImGui::InputInt((const char*)u8"Число вершин", &verticesCount, 1, 10);
	verticesCount = std::clamp(verticesCount, 2, 500);
	this->graph.setVerticesCount(verticesCount);

	if (ImGui::Button((const char*)u8"Импорт из файла"))
		ImGui::OpenPopup("file_import_popup");
	if (ImGui::BeginPopup("file_import_popup")) {
		ImGui::Text((const char*)u8"Какой-то импорт из файла...");
		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (ImGui::Button((const char*)u8"Рандомная генерация"))
		ImGui::OpenPopup("random_generation_popup");
	if (ImGui::BeginPopup("random_generation_popup")) {
		ImGui::Text((const char*)u8"Что-то рандомно происходит...");
		ImGui::EndPopup();
	}

	ImGui::SeparatorText((const char*)u8"Матрица смежности");

	const ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;
	const ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;

	verticesCount = this->graph.getVerticesCount();
	auto& adjacencyMatrix = this->graph.getAdjacencyMatrix();

	if (ImGui::BeginTable("adjacency_matrix_table", verticesCount + 1, tableFlags, ImVec2(0, 500))) {
		ImGui::TableSetupColumn("##", columnFlags);
		for (int column = 0; column < verticesCount; column++)
			ImGui::TableSetupColumn(std::to_string(column).c_str(), columnFlags, 25.0f);

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

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 origin = ImGui::GetCursorScreenPos();

	ImU32 edgeColor = IM_COL32(0, 255, 0, 255);
	float edgeThickness = 3;

	ImU32 vertexColor = IM_COL32(255, 255, 255, 255);
	ImU32 vertexTextColor = IM_COL32(0, 0, 0, 255);
	float vertexSize = 10;

	for (int startVertexIndex = 0; startVertexIndex < verticesCount; startVertexIndex++) {
		for (int endVertexIndex = startVertexIndex + 1; endVertexIndex < verticesCount; endVertexIndex++) {
			if (adjacencyMatrix.at(startVertexIndex).at(endVertexIndex) == 0)
				continue;
			Vertex startVertex = this->graph.getVertices().at(startVertexIndex);
			Vertex endVertex = this->graph.getVertices().at(endVertexIndex);
			drawList->AddLine(ImVec2(startVertex.x + origin.x, startVertex.y + origin.y), ImVec2(endVertex.x + origin.x, endVertex.y + origin.y), edgeColor, edgeThickness);
		}
	}

	for (int vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
		Vertex currentVertex = this->graph.getVertices().at(vertexIndex);
		drawList->AddCircleFilled(ImVec2(currentVertex.x + origin.x, currentVertex.y + origin.y), vertexSize, vertexColor);
		drawList->AddText(ImVec2(currentVertex.x - vertexSize + 5 + origin.x, currentVertex.y - vertexSize + 5 + origin.y), vertexTextColor, std::to_string(vertexIndex).c_str());
	}

	ImGuiIO& io = ImGui::GetIO();
	static int draggingVertexIndex = -1;

	if (io.MouseClicked[ImGuiMouseButton_Left] && draggingVertexIndex == -1) {
		for (int vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
			Vertex currentVertex = this->graph.getVertices().at(vertexIndex);
			float deltaX = origin.x + currentVertex.x - io.MousePos.x;
			float deltaY = origin.y + currentVertex.y - io.MousePos.y;
			if (deltaX * deltaX + deltaY * deltaY < vertexSize * vertexSize) {
				draggingVertexIndex = vertexIndex;
				break;
			}
		}
	}

	if (draggingVertexIndex != -1) {
		this->graph.getVertices().at(draggingVertexIndex).x = io.MousePos.x - origin.x;
		this->graph.getVertices().at(draggingVertexIndex).y = io.MousePos.y - origin.y;

		if (!io.MouseDown[ImGuiMouseButton_Left])
			draggingVertexIndex = -1;
	}

	ImGui::Dummy(ImVec2(200, 200));

	ImGui::End();
}