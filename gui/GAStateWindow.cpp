#include "GAStateWindow.h"
#include <imgui.h>
#include <implot.h>
#include <vector>

GAStateWindow::GAStateWindow() : epoch(17), best(17), avg(17)
{
	for (int i = 0; i < epoch.size(); i++) {
		epoch.at(i) = i + 1.0f;
		best.at(i) = 700 / static_cast<float>(i) + std::rand() % 20 - 10;
		avg.at(i) = 900 / static_cast<float>(i) + std::rand() % 20 - 10;
	}
}

void GAStateWindow::render()
{
	if (!ImGui::Begin((const char*)u8"Состояние ГА"))
		return ImGui::End();

	const int currentEpoch = 17;
	const int maxEpoch = 50;
	const float bestTargetFunctionValue = 732.0f;
	const float avgTargetFunctionValue = 1032.0f;

	ImGui::Text((const char*)u8"Эпоха: %d из %d", currentEpoch, maxEpoch);
	ImGui::Text((const char*)u8"Лучшее значение целевой функции: %.3f", bestTargetFunctionValue);
	ImGui::Text((const char*)u8"Среднее значение целевой функции: %.3f", avgTargetFunctionValue);

	if (ImPlot::BeginPlot((const char*)u8"##", ImVec2(-1, -1))) {
		ImPlot::SetupAxes((const char*)u8"Эпоха", (const char*)u8"F");

		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotLine((const char*)u8"Лучшее", this->epoch.data(), this->best.data(), this->epoch.size());

		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotLine((const char*)u8"Среднее", this->epoch.data(), this->avg.data(), this->epoch.size());

		ImPlot::EndPlot();
	}

	ImGui::End();
}
