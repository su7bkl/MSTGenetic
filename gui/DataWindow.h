#pragma once
#include "gui/WindowBase.h"
#include "dummy/Graph.h"

class DataWindow : public  WindowBase
{
private:
    Graph& graph;

public:
    DataWindow(Graph& graph);
    void render() override;
};
