#include "Genetic.h"

namespace genetic {
    std::mt19937 Chromosome::rng((std::random_device())());
    std::mt19937 Breeder::rng((std::random_device())());

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

    Chromosome::Chromosome(Graph& graph) {
        this->included.resize(graph.getVeretexCount());
        this->basicGraph = &graph;
    }

    Chromosome::Chromosome(Graph* graph, std::vector<bool>& selected) {
        if(graph->getEdgeCount() != selected.size()) {
            throw std::invalid_argument("Wrong selected edge array size");
        }
        this->included = selected;
        this->basicGraph = graph;
    }

    Graph Chromosome::getGraph() {
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
        return Graph(this->basicGraph->getVertices(), includedEdges);
    }

    Graph* Chromosome::getBasicGraph() {
        return this->basicGraph;
    }

    int Chromosome::getFitness() {
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
        fitness += (int)std::round(std::log(this->getGraph().getConnectedComponentsCount()) * this->basicGraph->getTotalEdgeLength());
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

    Breeder::Breeder(BreedingType type) {
        this->type = type;
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
}