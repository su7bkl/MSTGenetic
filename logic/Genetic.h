#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <list>
#include <compare>

#include "logic/Graph.h"

#define MUTATION_COUNT 1
#define CACHE_SIZE 1000

namespace genetic {
    enum BreedingType {
        SinglePoint,
        DoublePoint,
        Uniform
    };

    enum SelectionType {
        Roulette,
        RangedRoulette,
        UniformDist
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
        friend auto operator<=>(genetic::Chromosome& first, genetic::Chromosome& second) {
            return (first.getFitness() <=> second.getFitness());
        }
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
        std::pair<double, double> uniformRange;
        std::pair<double, double> stats = { -1.0, -1.0 };
        std::vector<Chromosome> entities;
        std::vector<double> fitnessCumulativeSums;
        Graph* basicGraph;
        int size;
        SelectionType selType;
        Breeder breeder;
        void breedSelectRoulette(double breedProb, double mutationProb);
        void breedSelectRangedRoulette(double breedProb, double mutationProb);
        void breedSelectUniform(double breedProb, double mutationProb);
        void computeFitnesses();
    public:
        Generation(int size, Graph* basicGraph, SelectionType selType, BreedingType breedType);
        Generation(Generation& prev, double breedProb, double mutationProb, std::pair<double, double> uniformRange);
        std::pair<Chromosome*, double> getEntity(int id);
        int getSize();
        //first is max, second is avg
        std::pair<double, double> getGenerationStats();
    };

    class GeneticAlgorithm {
    private:
        BreedingType breedingType;
        SelectionType selectionType;
        int generationSize;
        int currentGenerationNumber = 0;
        double breedingProbability;
        double mutationProbability;

        std::pair<double, double> uniformRange;

        std::list<Generation> generationBuffer;
        std::list<Generation>::iterator currentGeneration;

        int generationBufferLimit;
        int maxGeneration;

        Graph& graph;

        std::vector< std::pair<double, double>> stats;

        Breeder breeder;

        bool started;

    public:
        GeneticAlgorithm(Graph& graph, int generationBufferLimit);

        BreedingType getBreedingType();
        void setBreedingType(BreedingType breedingType);

        SelectionType getSelectionType();
        void setSelectionType(SelectionType selectionType);

        int getGenerationSize();
        void setGenerationSize(int generationSize);

        int getGenerationBufferLimit();
        void setGenerationBufferLimit(int newGenerationBufferLimit);

        double getBreedingProbability();
        void setBreedingProbability(double breedingProbability);

        double getMutationProbability();
        void setMutationProbability(double mutationProbability);

        bool isStarted();

        std::list<Generation>::iterator getCurrentGeneration();

        void start();
        void stop();
        void stepForward();
        void stepBack();
        void toEnd(int finalGen);

        const std::vector< std::pair<double, double>>& getStats();

        std::pair<double, double> getUniformRange();
        void setUniformRange(std::pair<double, double> newUniformRange);

        int getCurrentGenerationNumber();
    };
}

