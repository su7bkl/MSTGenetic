#include "gui/GAStateWindow.h"
#include <imgui.h>
#include <implot.h>

namespace gui {
    GAStateWindow::GAStateWindow(genetic::GeneticAlgorithm& geneticAlgorithm) : geneticAlgorithm(geneticAlgorithm)
    {}

    void GAStateWindow::render()
    {
        // окно
        if (!ImGui::Begin((const char*)u8"Состояние ГА"))
            return ImGui::End();

        // выход если алгоритм не запущен
        if (!this->geneticAlgorithm.isStarted()) {
            ImGui::Text((const char*)u8"Алгоритм не запущен!");
            return ImGui::End();
        }

        // значения параметров
        const int currentEpoch = 17;
        const int maxEpoch = 50;
        const float bestTargetFunctionValue = 732.0f;
        const float avgTargetFunctionValue = 1032.0f;

        // текст текущей эпохи
        ImGui::Text((const char*)u8"Эпоха: %d из %d", currentEpoch, maxEpoch);
        // текст лучшего значения функции
        ImGui::Text((const char*)u8"Лучшее значение целевой функции: %.3f", this->geneticAlgorithm.getCurrentGeneration()->getGenerationStats().first);
        // текст среднего значения функции
        ImGui::Text((const char*)u8"Среднее значение целевой функции: %.3f", this->geneticAlgorithm.getCurrentGeneration()->getGenerationStats().second);

        //// графики
        //if (ImPlot::BeginPlot((const char*)u8"##", ImVec2(-FLT_MIN, -FLT_MIN))) {
        //    ImPlot::SetupAxes((const char*)u8"Эпоха", (const char*)u8"F");
        //    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 1, this->epoch.size());

        //    // график лучшего значения по поколениям
        //    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        //    ImPlot::PlotLine((const char*)u8"Лучшее", this->epoch.data(), this->best.data(), this->epoch.size());

        //    // график среднего значения по поколениям
        //    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        //    ImPlot::PlotLine((const char*)u8"Среднее", this->epoch.data(), this->avg.data(), this->epoch.size());

        //    ImPlot::EndPlot();
        //}

        ImGui::End();
    }
}
