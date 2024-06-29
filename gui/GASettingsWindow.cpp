#include "GASettingsWindow.h"
#include "imgui.h"
#include <algorithm>

GASettingsWindow::GASettingsWindow()
{
}

void GASettingsWindow::render()
{
	if (!ImGui::Begin((const char*)u8"Настройки ГА"))
		return ImGui::End();

	static int populationSize = 1;
	static int epochCount = 100;
	static float mutationPobability = 0.5f;
	static float crossingPobability = 0.5f;

	static int crossingType = 0;
	static int selectionType = 0;

	const char* crossingTypeNames[] = { (const char*)u8"Одноточечное", (const char*)u8"Двухточечное", (const char*)u8"Равномерное" };
	const char* selectionTypeNames[] = { (const char*)u8"Рулетка", (const char*)u8"Ранжирование", (const char*)u8"Равномерное" };

	static float uniformSelectionBorders[2] = { 0.0f , 1.0f };

	ImGui::InputInt((const char*)u8"Размер популяции", &populationSize, 10, 100);
	populationSize = std::max(populationSize, 1);

	ImGui::InputInt((const char*)u8"Число эпох", &epochCount, 10, 100);
	epochCount = std::max(epochCount, 1);

	ImGui::Spacing();

	ImGui::SliderFloat((const char*)u8"Вероятность мутации", &mutationPobability, 0, 1);
	ImGui::SliderFloat((const char*)u8"Вероятность скрещивания", &crossingPobability, 0, 1);

	ImGui::Spacing();

	ImGui::Combo((const char*)u8"Вид скрещивания", &crossingType, crossingTypeNames, IM_ARRAYSIZE(crossingTypeNames));
	ImGui::Combo((const char*)u8"Вид отбора", &selectionType, selectionTypeNames, IM_ARRAYSIZE(selectionTypeNames));

	ImGui::Spacing();

	if (selectionType != 2)
		ImGui::BeginDisabled();

	ImGui::InputFloat2((const char*)u8"Диапазон значений при равномерном отборе", uniformSelectionBorders);
	uniformSelectionBorders[1] = std::max(uniformSelectionBorders[0] + 1.0f, uniformSelectionBorders[1]);

	if (selectionType != 2)
		ImGui::EndDisabled();

	ImGui::End();
}
