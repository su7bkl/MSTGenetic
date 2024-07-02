#pragma once
#include "gui/BaseWindow.h"
#include "dummy/Graph.h"

namespace gui {
    class DataWindow : public BaseWindow
    {
    private:
        Graph& graph;

    public:
        DataWindow(Graph& graph);
        void render() override;
    };
}
