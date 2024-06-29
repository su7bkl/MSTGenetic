#pragma once
#include "gui/WindowBase.h"
#include <vector>

class GAStateWindow : public WindowBase
{
private:
	std::vector<float> epoch;
	std::vector<float> best;
	std::vector<float> avg;

public:
	GAStateWindow();
	void render() override;
};

