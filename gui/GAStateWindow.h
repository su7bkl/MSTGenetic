#pragma once
#include "gui/BaseWindow.h"
#include <vector>

class GAStateWindow : public BaseWindow
{
private:
    // будет убрано
    std::vector<float> epoch;
    std::vector<float> best;
    std::vector<float> avg;

public:
    GAStateWindow();
    void render() override;
};
