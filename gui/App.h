#pragma once
#include "gui/DataWindow.h"
#include "gui/GASettingsWindow.h"
#include "gui/EpochWindow.h"
#include "gui/GAStateWindow.h"
#include "Graph.h"
#include "dummy/GeneticAlgorithm.h"

namespace gui {
    class App
    {
    private:
        bool disposed = false;
        bool running;

        genetic::Graph graph;
        GeneticAlgorithm geneticAlgorithm;

        DataWindow dataWindow;
        GASettingsWindow gaSettingWindow;
        EpochWindow epochWindow;
        GAStateWindow gaStateWindow;

    public:
        App();

        void render();
        void cleanUp();
        ~App();

        bool isRunning();
        void stop();
    };
}