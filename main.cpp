// Ihor Bondar K-25, gcc

#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <execution>
#include <thread>
#include <random>
#include "timeit"


std::vector<double> generateRandomVector(size_t size, double min_val = -100.0, double max_val = 100.0) {
    std::random_device rd;
    mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min_val, max_val);
    
    std::vector<double> vec(size);
    for(auto& elem : vec) {
        elem = dis(gen);
    }
    return vec;
}


void testStandardAlgorithms(const vector<double>& vec1, const vector<double>& vec2, int iterations = 1) {
    double result;

    std::cout << "inner product without policy \n";
    timeit([&vec1, &vec2, &result]() {
        result = inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    });

    std::cout << "transform reduce seq \n";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::seq,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);

    std::cout << "transform reduce par \n";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::par,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);

    std::cout << "transform reduce unseq \n";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::unseq,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);

    std::cout << "transform reduce par_unseq \n";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::par_unseq,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);
}


int main() {

    const int ITERATIONS_NUMBER = 100;

    const std::vector<size_t> sizes = {
        1000000,
        10000000,
        100000000
    };

    for (const size_t& size : sizes ) {
        std::vector<double> vec1 = generateRandomVector(size);
        std::vector<double> vec2 = generateRandomVector(size);
        std::cout << std::format("\n===================\nSize of generated test dataset: {} \n \n", size);
        testStandardAlgorithms(vec1, vec2, ITERATIONS_NUMBER);
    }

    return 0;
}