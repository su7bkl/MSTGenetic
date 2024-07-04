#include "gui/GASettingsWindow.h"
#include <imgui.h>
#include <algorithm>

namespace gui {
    GASettingsWindow::GASettingsWindow(genetic::GeneticAlgorithm& geneticAlgorithm) : geneticAlgorithm(geneticAlgorithm) {}

    void GASettingsWindow::render()
    {
        // окно
        if (!ImGui::Begin((const char*)u8"Настройки ГА"))
            return ImGui::End();

        // размер популяции
        {
            if (this->geneticAlgorithm.isStarted())
                ImGui::BeginDisabled();

            int populationSize = this->geneticAlgorithm.getGenerationSize();
            ImGui::InputInt((const char*)u8"Размер популяции", &populationSize, 10, 100);
            populationSize = std::max(populationSize, 1);
            this->geneticAlgorithm.setGenerationSize(populationSize);

            if (this->geneticAlgorithm.isStarted())
                ImGui::EndDisabled();
        }

        // число эпох
        ImGui::BeginDisabled();
        int epochCount = 100;
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
        const char* breedingTypeNames[] = { (const char*)u8"Одноточечное", (const char*)u8"Двухточечное", (const char*)u8"Равномерное" };
        int breedingType = this->geneticAlgorithm.getBreedingType();
        ImGui::Combo((const char*)u8"Вид скрещивания", &breedingType, breedingTypeNames, IM_ARRAYSIZE(breedingTypeNames));
        this->geneticAlgorithm.setBreedingType(static_cast<genetic::BreedingType>(breedingType));

        // вид отбора
        ImGui::BeginDisabled();
        const char* selectionTypeNames[] = { (const char*)u8"Рулетка", (const char*)u8"Ранжирование", (const char*)u8"Равномерное" };
        int selectionType = 0;
        ImGui::Combo((const char*)u8"Вид отбора", &selectionType, selectionTypeNames, IM_ARRAYSIZE(selectionTypeNames));
        this->geneticAlgorithm.setSelectionType(static_cast<genetic::SelectionType>(selectionType));
        ImGui::EndDisabled();

        ImGui::Spacing();

        // отключает пункт ниже если выбран другой вид отбора
        //if (selectionType != 2)
        ImGui::BeginDisabled();

        // границы значений при равномерном отборе
        float uniformSelectionBorders[2] = { 0.0f , 1.0f };
        ImGui::InputFloat2((const char*)u8"Диапазон значений при равномерном отборе", uniformSelectionBorders);
        uniformSelectionBorders[1] = std::max(uniformSelectionBorders[0] + 1.0f, uniformSelectionBorders[1]);

        //if (selectionType != 2)
        ImGui::EndDisabled();

        ImGui::End();
    }
}
