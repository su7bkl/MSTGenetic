#pragma once
#include "gui/BaseWindow.h"
#include "Genetic.h"
#include <vector>

namespace gui {
    class GAStateWindow : public BaseWindow
    {
    private:
        genetic::GeneticAlgorithm& geneticAlgorithm;

    public:
        GAStateWindow(genetic::GeneticAlgorithm& geneticAlgorithm);
        void render() override;
    };
}
