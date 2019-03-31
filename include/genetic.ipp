
#pragma once

#include <cstdlib>
#include <type_traits>

template <typename T, typename Arithmetic>
Genetic<T, Arithmetic>::Genetic
(
    std::function<T(const T&, const T&)>&& crossover,
    std::function<void(T&)>&& mutate,
    std::function<Arithmetic(const T&)>&& fitness
)
:
crossover(crossover),
mutate(mutate),
fitness(fitness)
{
    static_assert
    (
        std::is_floating_point<Arithmetic>::value,
        "Non floating point type given as the second template parameter"
    );
}

template <typename T, typename Arithmetic>
T Genetic<T, Arithmetic>::operator()
(
    std::vector<T> ancestors,
    std::function<bool(const T&)>&& condition, std::size_t maxIterations,
    Arithmetic mutationRate
) const
{
    auto& best = ancestors.front(); Arithmetic max = fitness(best);

    for (auto current = ++ancestors.begin(); current != ancestors.end(); ++current)
    {
        const Arithmetic value = fitness(*current);

        if (value > max)
        {
            max = value; best = *current;
        }
    }

    for (std::size_t it = 0UL; it < maxIterations && !condition(best); it++)
    {
        std::sort
        (
            ancestors.begin(), ancestors.end(),
            [this](const auto& A, const auto& B)
            {
                return fitness(A) > fitness(B);
            }
        );

        std::vector<T> successors;
        for (std::size_t total = 0UL; total < ancestors.size(); total++)
        {
            const auto& ancestor1 = ancestors[std::rand() % ancestors.size() / 2];
            const auto& ancestor2 = ancestors[std::rand() % ancestors.size() / 2];

            auto child = crossover(ancestor1, ancestor2);

            if (static_cast<Arithmetic>(std::rand()) / RAND_MAX < mutationRate)
                mutate(child);

            successors.push_back(child);

            const Arithmetic value = fitness(child);

            if (value > max)
            {
                max = value; best = child;
            }
        }

        ancestors = successors;
    }

    return best;
}
