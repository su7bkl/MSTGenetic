#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <random>
#include <algorithm>

#include "Graph.h"

#define MUTATION_COUNT 2

namespace genetic {
    enum BreedingType {
        SinglePoint,
        DoublePoint,
        Uniform
    };

    class Chromosome {
    private:
        static std::mt19937 rng;
        std::vector<bool> included;
        Graph* basicGraph;
        int getEdgeCount();
    public:
        Graph getGraph();
        Graph* getBasicGraph();
        int getFitness();
        int getSize();
        std::vector<bool> getIncluded();
        Chromosome(Graph& graph);
        Chromosome(Graph* graph, std::vector<bool>& selected);
        Chromosome mutate();
    };

    class Breeder {
    private:
        BreedingType type;
        static std::mt19937 rng;
        static std::pair<Chromosome, Chromosome> breedSinglePoint(Chromosome& a, Chromosome& b);
        static std::pair<Chromosome, Chromosome> breedDoublePoint(Chromosome& a, Chromosome& b);
        static std::pair<Chromosome, Chromosome> breedUniform(Chromosome& a, Chromosome& b);
    public:
        Breeder(BreedingType type);
        std::pair<Chromosome, Chromosome> breed(Chromosome& a, Chromosome& b);
    };
}