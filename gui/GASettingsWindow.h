#pragma once
#include "gui/BaseWindow.h"
#include "dummy/GeneticAlgorithm.h"

namespace gui {
    class GASettingsWindow : public BaseWindow
    {
    private:
        GeneticAlgorithm& geneticAlgorithm;

    public:
        GASettingsWindow(GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
