#include "gui/GAStateWindow.h"
#include <imgui.h>
#include <implot.h>

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
    // окно
    if (!ImGui::Begin((const char*)u8"Состояние ГА"))
        return ImGui::End();

    // значения параметров
    const int currentEpoch = 17;
    const int maxEpoch = 50;
    const float bestTargetFunctionValue = 732.0f;
    const float avgTargetFunctionValue = 1032.0f;

    // текст текущей эпохи
    ImGui::Text((const char*)u8"Эпоха: %d из %d", currentEpoch, maxEpoch);
    // текст лучшего значения функции
    ImGui::Text((const char*)u8"Лучшее значение целевой функции: %.3f", bestTargetFunctionValue);
    // текст среднего значения функции
    ImGui::Text((const char*)u8"Среднее значение целевой функции: %.3f", avgTargetFunctionValue);

    // графики
    if (ImPlot::BeginPlot((const char*)u8"##", ImVec2(-FLT_MIN, -FLT_MIN))) {
        ImPlot::SetupAxes((const char*)u8"Эпоха", (const char*)u8"F");

        // график лучшего значения по поколениям
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine((const char*)u8"Лучшее", this->epoch.data(), this->best.data(), this->epoch.size());

        // график среднего значения по поколениям
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine((const char*)u8"Среднее", this->epoch.data(), this->avg.data(), this->epoch.size());

        ImPlot::EndPlot();
    }

    ImGui::End();
}
