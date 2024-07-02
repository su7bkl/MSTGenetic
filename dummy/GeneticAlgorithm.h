#pragma once
#include "Genetic.h"
#include "Graph.h"
#include <vector>

class GeneticAlgorithm
{
private:
    genetic::BreedingType breedingType;
    genetic::SelectionType selectionType;
    int generationSize;
    float breedingProbability;
    float mutationProbability;

    genetic::Breeder breeder;
    genetic::Generation* currentGeneration;

    bool started;

    genetic::Graph& graph;

public:
    GeneticAlgorithm(genetic::Graph& graph);

    genetic::BreedingType getBreedingType();
    void setBreedingType(genetic::BreedingType breedingType);

    genetic::SelectionType getSelectionType();
    void setSelectionType(genetic::SelectionType selectionType);

    int getGenerationSize();
    void setGenerationSize(int generationSize);

    float getBreedingProbability();
    void setBreedingProbability(float breedingProbability);

    float getMutationProbability();
    void setMutationProbability(float mutationProbability);

    bool isStarted();

    genetic::Generation& getCurrentGeneration();

    void start();
    void stop();
    void step();

    ~GeneticAlgorithm();
};
