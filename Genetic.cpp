#include "Genetic.h"

namespace genetic {
    std::mt19937 Chromosome::rng((std::random_device())());
    std::mt19937 Breeder::rng((std::random_device())());
    std::mt19937 Generation::rng((std::random_device())());

    int Chromosome::getEdgeCount() {
        int res = 0;
        for(bool i : this->included) {
            if(i) res++;
        }
        return res;
    }

    int Chromosome::getSize() {
        return this->included.size();
    }

    Chromosome::Chromosome(Graph* graph) {
        int size = graph->getVeretexCount();
        std::bernoulli_distribution dist(0.5);
        this->included.resize(size);
        for(int i = 0; i < size; i++) {
            this->included[i] = dist(this->rng);
        }
        this->basicGraph = graph;
    }

    Chromosome::Chromosome(Graph* graph, std::vector<bool>& selected) {
        if(graph->getEdgeCount() != selected.size()) {
            throw std::invalid_argument("Wrong selected edge array size");
        }
        this->included = selected;
        this->basicGraph = graph;
    }

    Graph* Chromosome::getGraph() {
        if(this->correspondingGraph.getVeretexCount() != 0) {
            return &this->correspondingGraph;
        }
        int edgeId = 0;
        std::vector<Edge> includedEdges;
        for(int i = 1; i < this->basicGraph->getVeretexCount(); i++) {
            for(int j = 0; j < i; j++) {
                if(this->basicGraph->getEgdeLength({ i, j }) != 0) {
                    if(this->included[edgeId]) {
                        includedEdges.push_back({ {i, j}, this->basicGraph->getEgdeLength({ i, j }) });
                    }
                    edgeId++;
                }
            }
        }
        this->correspondingGraph = Graph(this->basicGraph->getVertices(), includedEdges);
        return &this->correspondingGraph;
    }

    Graph* Chromosome::getBasicGraph() {
        return this->basicGraph;
    }

    double Chromosome::getFitness() {
        int edgeId = 0;
        int fitness = 0;
        for(int i = 1; i < this->basicGraph->getVeretexCount(); i++) {
            for(int j = 0; j < i; j++) {
                if(this->basicGraph->getEgdeLength({ i, j }) != 0) {
                    if(this->included[edgeId]) {
                        fitness += this->basicGraph->getEgdeLength({ i, j });
                    }
                    edgeId++;
                }
            }
        }
        if(this->basicGraph->getVeretexCount() - 1 < this->getEdgeCount()) {
            fitness += (int)std::round(std::log(this->getEdgeCount() - (this->basicGraph->getVeretexCount() - 1) + 1) *
                ((double)this->basicGraph->getTotalEdgeLength() / (double)this->basicGraph->getEdgeCount()));
        }
        fitness += 1.0 / (std::log(this->getGraph()->getConnectedComponentsCount()) * this->basicGraph->getTotalEdgeLength());
        return fitness;
    }

    std::vector<bool> Chromosome::getIncluded() {
        return this->included;
    }

    Chromosome Chromosome::mutate() {
        std::vector<bool> newIncluded = this->included;
        std::uniform_int_distribution<int> dist(0, this->included.size() - 1);
        for(int i = 0; i < MUTATION_COUNT; i++) {
            newIncluded[dist(this->rng)] = !newIncluded[i];
        }
        return Chromosome(this->basicGraph, newIncluded);
    }

    Breeder::Breeder() {
        this->type = SinglePoint;
    }

    Breeder::Breeder(BreedingType type) {
        this->type = type;
    }

    void Breeder::setBreedingType(BreedingType newType) {
        this->type = newType;
    }

    std::pair<Chromosome, Chromosome> Breeder::breed(Chromosome& a, Chromosome& b) {
        if(a.getSize() != b.getSize()) {
            throw std::invalid_argument("Different chromosome sizes to breed");
        }
        switch(this->type) {
        case SinglePoint:
            return Breeder::breedSinglePoint(a, b);
        case DoublePoint:
            return Breeder::breedDoublePoint(a, b);
        case Uniform:
            return Breeder::breedUniform(a, b);
        default:
            return Breeder::breedSinglePoint(a, b);
        }
    }

    std::pair<Chromosome, Chromosome> Breeder::breedSinglePoint(Chromosome& a, Chromosome& b) {
        using std::copy;
        int size = a.getSize();
        if(size < 2) {
            if((std::bernoulli_distribution(0.5))(Breeder::rng)) {
                return { a, b };
            }
            return { b, a };
        }
        std::uniform_int_distribution<int> dist(1, size - 1);
        int cutPoint = dist(Breeder::rng);
        std::vector<bool> child1Included(size), child2Included(size);
        copy(a.getIncluded().begin(),
            a.getIncluded().begin() + cutPoint,
            child1Included.begin());
        copy(b.getIncluded().begin() + cutPoint,
            a.getIncluded().end(),
            child1Included.begin() + cutPoint);
        copy(b.getIncluded().begin(),
            b.getIncluded().begin() + cutPoint,
            child2Included.begin());
        copy(a.getIncluded().begin() + cutPoint,
            a.getIncluded().end(),
            child2Included.begin() + cutPoint);
        Chromosome child1(a.getBasicGraph(), child1Included);
        Chromosome child2(a.getBasicGraph(), child2Included);
        return { child1, child2 };
    }

    std::pair<Chromosome, Chromosome> Breeder::breedDoublePoint(Chromosome& a, Chromosome& b) {
        using std::copy;
        int size = a.getSize();
        if(size < 3) {
            return Breeder::breedSinglePoint(a, b);
        }
        std::uniform_int_distribution<int> dist1(1, size - 2);
        int cutPoint1 = dist1(Breeder::rng);
        std::uniform_int_distribution<int> dist2(cutPoint1, size - 1);
        int cutPoint2 = dist2(Breeder::rng);
        std::vector<bool> child1Included = a.getIncluded();
        std::vector<bool> child2Included = b.getIncluded();
        copy(b.getIncluded().begin() + cutPoint1,
            b.getIncluded().begin() + cutPoint2,
            child1Included.begin() + cutPoint1);
        copy(a.getIncluded().begin() + cutPoint1,
            a.getIncluded().begin() + cutPoint2,
            child2Included.begin() + cutPoint1);
        Chromosome child1(a.getBasicGraph(), child1Included);
        Chromosome child2(a.getBasicGraph(), child2Included);
        return { child1, child2 };
    }

    std::pair<Chromosome, Chromosome> Breeder::breedUniform(Chromosome& a, Chromosome& b) {
        int size = a.getSize();
        std::bernoulli_distribution dist(0.5);
        std::vector<bool> child1Included(size), child2Included(size),
            aIncluded = a.getIncluded(), bIncluded = b.getIncluded();
        for(int i = 0; i < size; i++) {
            if(dist(Breeder::rng)) {
                child1Included[i] = aIncluded[i];
                child2Included[i] = bIncluded[i];
            }
            else {
                child1Included[i] = bIncluded[i];
                child2Included[i] = aIncluded[i];
            }
        }
        Chromosome child1(a.getBasicGraph(), child1Included);
        Chromosome child2(a.getBasicGraph(), child2Included);
        return { child1, child2 };
    }

    Generation::Generation(int size, Graph* basicGraph, SelectionType selType, BreedingType breedType) {
        this->selType = selType;
        this->breeder.setBreedingType(breedType);
        this->size = size;
        this->fitnesses.resize(size);
        this->basicGraph = basicGraph;
        for(int i = 0; i < size; i++) {
            this->entities.push_back(Chromosome(basicGraph));
        }
        this->computeFitnesses();
    }

    Generation::Generation(Generation& prev, double breedProb, double mutationProb) {
        this->size = prev.size;
        this->selType = prev.selType;
        this->basicGraph = prev.basicGraph;
        this->breeder = prev.breeder;
        this->entities = prev.entities;
        this->fitnesses = prev.fitnesses;
        switch(this->selType) {
        case Roulette:
            breedSelectRoulette(breedProb, mutationProb);
            break;
        default:
            break;
        }
        this->computeFitnesses();
    }

    void Generation::breedSelectRoulette(double breedProb, double mutationProb) {
        std::vector<Chromosome> selected;
        std::uniform_real_distribution<double> roulette(0, this->fitnesses[this->size - 1]);
        for(int i = 0; i < this->size; i++) {
            int pos = std::lower_bound(this->fitnesses.begin(), this->fitnesses.end(), roulette(this->rng)) - this->fitnesses.begin();
            selected.push_back(this->entities[pos]);
        }
        std::bernoulli_distribution breed(breedProb);
        std::bernoulli_distribution mutation(mutationProb);
        for(int i = 0; i < this->size / 2; i++) {
            if(breed(this->rng)) {
                std::pair<Chromosome, Chromosome> breedingResult = this->breeder.breed(selected[i], selected[i + 1]);
                if(mutation(this->rng)) {
                    selected[i] = breedingResult.first.mutate();
                }
                else {
                    selected[i] = breedingResult.first;
                }
                if(mutation(this->rng)) {
                    selected[i + 1] = breedingResult.second.mutate();
                }
                else {
                    selected[i + 1] = breedingResult.second;
                }
            }
        }
        this->entities = selected;
    }

    int Generation::getSize() {
        return this->size;
    }

    void Generation::computeFitnesses() {
        this->fitnesses[0] = this->entities[0].getFitness();
        for(int i = 1; i < size; i++) {
            this->fitnesses[i] = this->fitnesses[i - 1] + this->entities[i].getFitness();
        }
    }

    std::pair<Chromosome*, int> Generation::getEntity(int id) {
        if(id < 0 || id >= this->size) {
            throw std::invalid_argument("ID is bigger than generation size or negative");
        }
        if(id == 0)
            return { &this->entities[id], this->fitnesses[id] };
        else
            return { &this->entities[id], this->fitnesses[id] - this->fitnesses[id - 1] };
    }
}