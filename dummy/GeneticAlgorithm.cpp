#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(genetic::Graph& graph) :
    graph(graph),
    breedingType(genetic::BreedingType::SinglePoint),
    breeder(this->breedingType),
    started(false), generationSize(1),
    selectionType(genetic::SelectionType::Roulette),
    breedingProbability(0.5),
    mutationProbability(0.5),
    currentGeneration(nullptr)
{
}

genetic::BreedingType GeneticAlgorithm::getBreedingType()
{
    return this->breedingType;
}

void GeneticAlgorithm::setBreedingType(genetic::BreedingType breedingType)
{
    this->breedingType = breedingType;
    this->breeder.setBreedingType(breedingType);
}

genetic::SelectionType GeneticAlgorithm::getSelectionType()
{
    return this->selectionType;
}

void GeneticAlgorithm::setSelectionType(genetic::SelectionType selectionType)
{
    this->selectionType = selectionType;
}

int GeneticAlgorithm::getGenerationSize()
{
    return this->generationSize;
}

void GeneticAlgorithm::setGenerationSize(int generationSize)
{
    this->generationSize = generationSize;
}

float GeneticAlgorithm::getBreedingProbability()
{
    return this->breedingProbability;
}

void GeneticAlgorithm::setBreedingProbability(float breedingProbability)
{
    this->breedingProbability = breedingProbability;
}

float GeneticAlgorithm::getMutationProbability()
{
    return this->mutationProbability;
}

void GeneticAlgorithm::setMutationProbability(float mutationProbability)
{
    this->mutationProbability = mutationProbability;
}

bool GeneticAlgorithm::isStarted()
{
    return this->started;
}

genetic::Generation& GeneticAlgorithm::getCurrentGeneration()
{
    if (this->currentGeneration == nullptr)
        throw std::logic_error("No running!");

    return *this->currentGeneration;
}

void GeneticAlgorithm::start()
{
    if (this->started)
        return;

    this->started = true;
    this->currentGeneration = new genetic::Generation(this->generationSize, &this->graph, this->selectionType, this->breedingType);
}

void GeneticAlgorithm::stop()
{
    if (!this->started)
        return;

    this->started = false;
    delete this->currentGeneration;
    this->currentGeneration = nullptr;
}

void GeneticAlgorithm::step()
{
    if (!this->started)
        return;

    auto* newGeneration = new genetic::Generation(*this->currentGeneration, this->breedingProbability, this->mutationProbability);
    delete this->currentGeneration;
    this->currentGeneration = newGeneration;
}



GeneticAlgorithm::~GeneticAlgorithm()
{
    if (this->currentGeneration == nullptr)
        return;

    delete this->currentGeneration;
    this->currentGeneration = nullptr;
}
