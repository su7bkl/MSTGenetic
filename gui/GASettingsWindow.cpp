#include "gui/GASettingsWindow.h"
#include <imgui.h>
#include <algorithm>

namespace gui {
    GASettingsWindow::GASettingsWindow(GeneticAlgorithm& geneticAlgorithm) : geneticAlgorithm(geneticAlgorithm) {}

    void GASettingsWindow::render()
    {
        // окно
        if (!ImGui::Begin((const char*)u8"Настройки ГА"))
            return ImGui::End();

        // переменные изменяемых параметров
        static int epochCount = 100;

        static int crossingType = 0;
        static int selectionType = 0;

        // имена типов скрещивания и селекции
        const char* crossingTypeNames[] = { (const char*)u8"Одноточечное", (const char*)u8"Двухточечное", (const char*)u8"Равномерное" };
        const char* selectionTypeNames[] = { (const char*)u8"Рулетка", (const char*)u8"Ранжирование", (const char*)u8"Равномерное" };

        // границы значений при равномерном отборе
        static float uniformSelectionBorders[2] = { 0.0f , 1.0f };

        // размер популяции
        if (this->geneticAlgorithm.isStarted())
            ImGui::BeginDisabled();

        int populationSize = this->geneticAlgorithm.getGenerationSize();
        ImGui::InputInt((const char*)u8"Размер популяции", &populationSize, 10, 100);
        populationSize = std::max(populationSize, 1);
        this->geneticAlgorithm.setGenerationSize(populationSize);

        if (this->geneticAlgorithm.isStarted())
            ImGui::EndDisabled();

        // число эпох
        ImGui::BeginDisabled();
        ImGui::InputInt((const char*)u8"Число эпох", &epochCount, 10, 100);
        epochCount = std::max(epochCount, 1);
        ImGui::EndDisabled();

        ImGui::Spacing();

        // вероятность мутации
        float mutationPobability = this->geneticAlgorithm.getMutationProbability();
        ImGui::SliderFloat((const char*)u8"Вероятность мутации", &mutationPobability, 0, 1);
        this->geneticAlgorithm.setMutationProbability(mutationPobability);
        // вероятность скрещивания
        float crossingPobability = this->geneticAlgorithm.getBreedingProbability();
        ImGui::SliderFloat((const char*)u8"Вероятность скрещивания", &crossingPobability, 0, 1);
        this->geneticAlgorithm.setBreedingProbability(crossingPobability);

        ImGui::Spacing();

        // вид скрещивания
        int breedingType = this->geneticAlgorithm.getBreedingType();
        ImGui::Combo((const char*)u8"Вид скрещивания", &breedingType, crossingTypeNames, IM_ARRAYSIZE(crossingTypeNames));
        this->geneticAlgorithm.setBreedingType(static_cast<genetic::BreedingType>(breedingType));
        // вид отбора
        ImGui::BeginDisabled();
        ImGui::Combo((const char*)u8"Вид отбора", &selectionType, selectionTypeNames, IM_ARRAYSIZE(selectionTypeNames));
        ImGui::EndDisabled();

        ImGui::Spacing();

        // отключает пункт ниже если выбран другой вид отбора
        //if (selectionType != 2)
        ImGui::BeginDisabled();

        // границы значений при равномерном отборе
        ImGui::InputFloat2((const char*)u8"Диапазон значений при равномерном отборе", uniformSelectionBorders);
        uniformSelectionBorders[1] = std::max(uniformSelectionBorders[0] + 1.0f, uniformSelectionBorders[1]);

        //if (selectionType != 2)
        ImGui::EndDisabled();

        ImGui::End();
    }
}
