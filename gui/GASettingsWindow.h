#pragma once
#include "gui/BaseWindow.h"
#include "Genetic.h"

namespace gui {
    class GASettingsWindow : public BaseWindow
    {
    private:
        genetic::GeneticAlgorithm& geneticAlgorithm;

    public:
        GASettingsWindow(genetic::GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
