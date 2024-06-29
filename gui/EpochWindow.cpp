#include "gui/EpochWindow.h"
#include <imgui.h>
#include <algorithm>
#include <functional>
#include <string>
#include <format>

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
	if (sortSpecs == nullptr || (!sortSpecs->SpecsDirty && !this->chromosomesSortingRequired))
		return;

	std::sort(this->chromosomesOrder.begin(), this->chromosomesOrder.end(), std::bind(&EpochWindow::sortComparator, this, sortSpecs, std::placeholders::_1, std::placeholders::_2));

	sortSpecs->SpecsDirty = false;
	this->chromosomesSortingRequired = false;
}

bool EpochWindow::sortComparator(ImGuiTableSortSpecs* sortSpecs, const int& left, const int& right) const
{
	bool comparisionResult = false;

	if (sortSpecs->Specs[0].ColumnIndex == 0) {
		comparisionResult = left < right;
	}
	else {
		comparisionResult = this->chromosomes[left].getTargetFunctionValue() < this->chromosomes[right].getTargetFunctionValue();
	}

	if (sortSpecs->Specs[0].SortDirection == ImGuiSortDirection_Descending)
		comparisionResult = !comparisionResult;

	return comparisionResult;
}

EpochWindow::EpochWindow(Graph& graph) : graph(graph), chromosomesSortingRequired(false), selectedChromosome(-1)
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

void EpochWindow::render()
{
	if (!ImGui::Begin((const char*)u8"Информация об эпохе"))
		return ImGui::End();
	
	static int selectedChromosome = -1;

	ImGui::Button((const char*)u8"До конца");
	ImGui::SameLine();
	if (ImGui::Button((const char*)u8"Эпоха вперёд")) {
		selectedChromosome = -1;
		this->regenerateChromosomes();
	}
	ImGui::SameLine();
	ImGui::Button((const char*)u8"Эпоха назад");

	ImGui::SameLine();
	ImGui::Button((const char*)u8"Остановить");

	const ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable;
	const ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_None;

	if (ImGui::BeginTable("chromosomes_table", 2, tableFlags, ImVec2(0, 200))) {
		ImGui::TableSetupColumn((const char*)u8"ID", columnFlags);
		ImGui::TableSetupColumn((const char*)u8"Значение целевой функции", columnFlags);

		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		this->sortChromosomes(ImGui::TableGetSortSpecs());

		for (const auto& row : this->chromosomesOrder) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Selectable(std::to_string(row).c_str(), row == selectedChromosome, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap))
				selectedChromosome = row == selectedChromosome ? -1 : row;

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.3f", this->chromosomes.at(row).getTargetFunctionValue());
		}
		ImGui::EndTable();
	}

	if (selectedChromosome == -1)
		return ImGui::End();

	ImGui::SeparatorText((const char*)u8"Информация о выбранной хромосоме");

	ImGui::Text((const char*)u8"Геном");

	const ImGuiTableFlags genomeTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit;
	const ImGuiTableColumnFlags genomeColumnFlags = ImGuiTableColumnFlags_None;

	if (ImGui::BeginTable("genome_table", this->edges.size() + 1, genomeTableFlags, ImVec2(0, 50))) {
		ImGui::TableSetupColumn((const char*)u8"Ребро", genomeColumnFlags);
		for (const auto& edge : this->edges)
			ImGui::TableSetupColumn(std::format("{},{}", edge.first, edge.second).c_str(), genomeColumnFlags);

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
	ImGui::AlignTextToFramePadding();
	ImGui::Text((const char*)u8"Связность графа:");
	ImGui::SameLine();
	ImGui::BeginDisabled();
	bool isConnected = this->chromosomes.at(selectedChromosome).getIsConnected();
	ImGui::Checkbox("##", &isConnected);
	ImGui::EndDisabled();

	ImGui::Text((const char*)u8"Представление подграфа:");

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 origin = ImGui::GetCursorScreenPos();

	ImU32 edgeColor = IM_COL32(255, 255, 255, 255);
	ImU32 includedEdgeColor = IM_COL32(0, 255, 0, 255);
	float edgeThickness = 3;

	ImU32 vertexColor = IM_COL32(255, 255, 255, 255);
	ImU32 vertexTextColor = IM_COL32(0, 0, 0, 255);
	float vertexSize = 10;

	for (int edgeIndex = 0; edgeIndex < this->edges.size(); edgeIndex++) {
		Vertex startVertex = this->graph.getVertices().at(this->edges.at(edgeIndex).first);
		Vertex endVertex = this->graph.getVertices().at(this->edges.at(edgeIndex).second);
		drawList->AddLine(ImVec2(startVertex.x + origin.x, startVertex.y + origin.y), ImVec2(endVertex.x + origin.x, endVertex.y + origin.y), this->chromosomes.at(selectedChromosome).getGenome().at(edgeIndex) ? includedEdgeColor : edgeColor, edgeThickness);
	}

	for (int vertexIndex = 0; vertexIndex < this->graph.getVerticesCount(); vertexIndex++) {
		Vertex currentVertex = this->graph.getVertices().at(vertexIndex);
		drawList->AddCircleFilled(ImVec2(currentVertex.x + origin.x, currentVertex.y + origin.y), vertexSize, vertexColor);
		drawList->AddText(ImVec2(currentVertex.x - vertexSize + 5 + origin.x, currentVertex.y - vertexSize + 5 + origin.y), vertexTextColor, std::to_string(vertexIndex).c_str());
	}

	ImGui::End();
}