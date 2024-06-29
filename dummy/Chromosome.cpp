#include "dummy/Chromosome.h"

Chromosome::Chromosome(const Graph& graph) : graph(graph)
{
    int edgesCount = 0;

    for (int startVertexIndex = 0; startVertexIndex < this->graph.getVerticesCount(); startVertexIndex++)
        for (int endVertexIndex = startVertexIndex + 1; endVertexIndex < this->graph.getVerticesCount(); endVertexIndex++)
            if (this->graph.getEdge(startVertexIndex, endVertexIndex) > 0)
                edgesCount++;

    this->genome.resize(edgesCount);
    for (auto gen : this->genome)
        gen = std::rand() % 2  == 0;

    this->targetFunctionValue = std::rand() % 5000 / 2.0f;
    this->isConnected = std::rand() % 2 == 0;
}

const Graph& Chromosome::getGraph() const
{
    return this->graph;
}

const std::vector<bool>& Chromosome::getGenome() const
{
    return this->genome;
}

float Chromosome::getTargetFunctionValue() const
{
    return this->targetFunctionValue;
}

bool Chromosome::getIsConnected() const
{
    return this->isConnected;
}
