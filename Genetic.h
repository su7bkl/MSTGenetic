#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <unordered_set>

#include "Graph.h"

#define MUTATION_COUNT 1
#define CACHE_SIZE 1000

namespace genetic {
    enum BreedingType {
        SinglePoint,
        DoublePoint,
        Uniform
    };

    enum SelectionType {
        Roulette
    };

    class Chromosome {
    private:
        static std::mt19937 rng;
        std::vector<bool> included;
        Graph* basicGraph;
        Graph correspondingGraph;
        int getEdgeCount();
    public:
        Graph* getGraph();
        Graph* getBasicGraph();
        double getFitness();
        int getSize();
        std::vector<bool> getIncluded();
        Chromosome(Graph* graph);
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
        Breeder();
        Breeder(BreedingType type);
        void setBreedingType(BreedingType newType);
        std::pair<Chromosome, Chromosome> breed(Chromosome& a, Chromosome& b);
    };

    class Generation {
    private:
        static std::mt19937 rng;
        std::vector<Chromosome> entities;
        std::vector<double> fitnesses;
        Graph* basicGraph;
        int size;
        SelectionType selType;
        Breeder breeder;
        void breedSelectRoulette(double breedProb, double mutationProb);
        void computeFitnesses();
    public:
        Generation(int size, Graph* basicGraph, SelectionType selType, BreedingType breedType);
        Generation(Generation& prev, double breedProb, double mutationProb);
        std::pair<Chromosome*, double> getEntity(int id);
        int getSize();
    };
}
