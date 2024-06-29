#include "gui/App.h"
#include <imgui.h>
#include <implot.h>

App::App() : running(true), graph(2), dataWindow(graph), gaSettingWindow(), epochWindow(graph), gaStateWindow()
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("fonts/consolas.ttf", 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

void App::render()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

#ifdef _DEBUG
	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();
#endif

	this->dataWindow.render();
	this->gaSettingWindow.render();
	this->epochWindow.render();
	this->gaStateWindow.render();
}

void App::cleanUp()
{
	if (this->disposed)
		return;

	this->disposed = true;
	this->stop();

	this->dataWindow.cleanUp();
	this->gaSettingWindow.cleanUp();
	this->epochWindow.cleanUp();
	this->gaStateWindow.cleanUp();
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
