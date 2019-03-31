
#include <genetic.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <fstream>

#define POPULATION_SIZE      (100UL)
#define MAX_ITERATIONS       (10000UL)
#define MUTATION_PROBABILITY (0.3f)
#define MUTATION_RATE        (0.1f)

static const char TARGET[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
static const char LETTER[] = " ,.aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ0123456789";

struct Individual : public std::string
{
    float score;

    Individual()
    :
    std::string((sizeof(TARGET) - 1UL), '*'), score(0.0f)
    {
        for (auto& letter : *this)
            letter = LETTER[std::rand() % sizeof(LETTER)];

        for (std::size_t i = 0UL; i < size(); i++)
            score += static_cast<float>(at(i) == TARGET[i]) / size();
    }

    Individual(const Individual& other)
    :
    std::string(other), score(other.score)
    {
    }

    Individual(Individual&& other) noexcept
    :
    std::string(std::move(other)), score(std::move(other.score))
    {
    }

    Individual& operator=(const Individual& other)
    {
        std::string::operator=(other);

        score = other.score;

        return *this;
    }

    Individual& operator=(Individual&& other) noexcept
    {
        std::string::operator=(std::move(other));

        score = std::move(other.score);

        return *this;
    }

    Individual(const Individual& A, const Individual& B)
    :
    std::string((sizeof(TARGET) - 1UL), '*'), score(0.0f)
    {
        const std::size_t midpoint = (sizeof(TARGET) - 1UL) / 2UL;

        for (std::size_t i = 0UL; i < size(); i++)
            at(i) = (i <= midpoint ? A.at(i) : B.at(i));

        for (std::size_t i = 0UL; i < size(); i++)
            score += static_cast<float>(at(i) == TARGET[i]) / size();
    }

    void mutate()
    {
        for (auto& letter : *this)
            if (static_cast<float>(std::rand()) / RAND_MAX < MUTATION_RATE)
                letter = LETTER[std::rand() % sizeof(LETTER)];

        score = 0.0f;
        for (std::size_t i = 0UL; i < size(); i++)
            score += static_cast<float>(at(i) == TARGET[i]) / size();
    }

    friend std::ostream& operator<<(std::ostream& os, const Individual& individual)
    {
        return os
        << "{ string: '" << static_cast<std::string>(individual)
        << "', score: " << std::fixed << individual.score << " }";
    }
};

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    auto crossover = [](const Individual& A, const Individual& B) -> Individual
    {
        return Individual(A, B);
    };

    auto mutate = [](Individual& individual) -> void
    {
        individual.mutate();
    };

    auto fitness = [](const Individual& individual) -> float
    {
        return individual.score;
    };

    std::vector<Individual> population(POPULATION_SIZE, Individual());

    auto condition = [&fitness](const Individual& individual)
    {
        return std::abs(fitness(individual) - 1.0f) < 1.0e-6;
    };

    Genetic<Individual> genetic(crossover, mutate, fitness);

    const auto best = genetic(population, condition, MAX_ITERATIONS, MUTATION_PROBABILITY);

    std::cout << best << std::endl;

    return 0;
}
