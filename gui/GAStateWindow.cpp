#include "gui/GAStateWindow.h"
#include <vector>
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

        // текст текущей эпохи
        ImGui::Text((const char*)u8"Эпоха: %d из %d", this->geneticAlgorithm.getCurrentGenerationNumber(), this->geneticAlgorithm.getStats().size());
        // текст лучшего значения функции
        ImGui::Text((const char*)u8"Лучшее значение целевой функции: %.6f", this->geneticAlgorithm.getCurrentGeneration()->getGenerationStats().first);
        // текст среднего значения функции
        ImGui::Text((const char*)u8"Среднее значение целевой функции: %.6f", this->geneticAlgorithm.getCurrentGeneration()->getGenerationStats().second);

        // графики
        if (ImPlot::BeginPlot((const char*)u8"##", ImVec2(-FLT_MIN, -FLT_MIN))) {
            // более удобное представление данных
            static std::vector<float> epoches;
            static std::vector<float> best;
            static std::vector<float> avg;

            // если данные обновились, то график подстраивается под данные
            bool dataChangedFlag = false;
            static size_t dataSize = UINT64_MAX;

            dataChangedFlag = this->geneticAlgorithm.getStats().size() != dataSize;
            dataSize = this->geneticAlgorithm.getStats().size();

            // заполнение данных
            epoches.resize(dataSize);
            best.resize(dataSize);
            avg.resize(dataSize);

            for (int i = 0; i < dataSize; i++) {
                epoches.at(i) = i + 1;
                best.at(i) = this->geneticAlgorithm.getStats().at(i).first;
                avg.at(i) = this->geneticAlgorithm.getStats().at(i).second;
            }

            // установка параметров осей
            ImPlot::SetupAxes((const char*)u8"Эпоха", (const char*)u8"F", dataChangedFlag ? ImPlotAxisFlags_AutoFit : ImPlotAxisFlags_None, dataChangedFlag ? ImPlotAxisFlags_AutoFit : ImPlotAxisFlags_None);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 1, dataSize == 1 ? 2 : static_cast<double>(dataSize));
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 1);

            // график лучшего значения по поколениям
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine((const char*)u8"Лучшее", epoches.data(), best.data(), dataSize);

            // график среднего значения по поколениям
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine((const char*)u8"Среднее", epoches.data(), avg.data(), dataSize);

            ImPlot::EndPlot();
        }

        ImGui::End();
    }
}
