#pragma once
#include "dummy/Graph.h"
#include <vector>

class Chromosome
{
private:
    const Graph& graph;
    std::vector<bool> genome;
    float targetFunctionValue;
    bool isConnected;

public:
    Chromosome(const Graph& graph);

    const Graph& getGraph() const;
    const std::vector<bool>& getGenome() const;
    float getTargetFunctionValue() const;
    bool getIsConnected() const;
};
