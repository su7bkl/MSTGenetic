#include "gui/App.h"
#include <imgui.h>
#include <implot.h>

namespace gui {
    App::App() : running(true), graph(), geneticAlgorithm(this->graph), dataWindow(this->graph, this->geneticAlgorithm), gaSettingWindow(this->geneticAlgorithm), epochWindow(this->graph, this->geneticAlgorithm), gaStateWindow()
    {
        // минимум две вершины
        this->graph.addVertex(genetic::Vertex());
        this->graph.addVertex(genetic::Vertex());

        // установка флагов imgui
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigViewportsNoTaskBarIcon = true;

        // установка тёмной темы
        ImGui::StyleColorsDark();

        // загрузка шрифта для кириллических символов
        io.Fonts->AddFontFromFileTTF("fonts/consolas.ttf", 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }

    void App::render()
    {
        // включение режима докинга на главном окне
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

#ifdef _DEBUG
        // показать окна с референсом GUI компонентов imgui И implot
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
#endif

        // показ всех окон
        this->dataWindow.render();
        this->gaSettingWindow.render();
        this->epochWindow.render();
        this->gaStateWindow.render();
    }

    void App::cleanUp()
    {
        if (this->disposed)
            return;

        this->stop();

        // очистка ресурсов всех оконы
        this->dataWindow.cleanUp();
        this->gaSettingWindow.cleanUp();
        this->epochWindow.cleanUp();
        this->gaStateWindow.cleanUp();

        this->disposed = true;
    }

    App::~App()
    {
        this->cleanUp();
    }

    bool App::isRunning()
    {
        return this->running;
    }

    void App::stop()
    {
        this->running = false;
    }
}
