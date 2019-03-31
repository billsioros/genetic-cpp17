
#pragma once

#include <vector>
#include <functional>

template <typename T, typename Arithmetic = float>
class Genetic
{
    std::function<T(const T&, const T&)> crossover;
    std::function<void(T&)> mutate;
    std::function<Arithmetic(const T&)> fitness;

public:

    Genetic
    (
        std::function<T(const T&, const T&)>&& crossover,
        std::function<void(T&)>&& mutate,
        std::function<Arithmetic(const T&)>&& fitness
    );

    T operator()
    (
        std::vector<T> population,
        std::function<bool(const T&)>&& condition, std::size_t maxIterations,
        Arithmetic mutationRate
    ) const;
};

#include <genetic.ipp>
