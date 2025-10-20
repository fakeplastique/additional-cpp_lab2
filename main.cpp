// Ihor Bondar K-25, gcc

#include <iostream>
#include <vector>
#include <numeric>
#include <execution>
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


template<typename Iterator, typename T>
struct inner_product_block {
    void operator()(Iterator first1, Iterator last1, Iterator first2, T& result) {
        result = inner_product(first1, last1, first2, T(0));
    }
};

template<typename Iterator, typename T>
T parallel_inner_product(Iterator first1, Iterator last1, Iterator first2, T init, unsigned long num_threads) {
    size_t length = distance(first1, last1);
    if (!length) return init;

    if (num_threads <= 1 || length < num_threads) {
        return inner_product(first1, last1, first2, init);
    }

    size_t block_size = length / num_threads;
    std::vector<T> results(num_threads, 0);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start1 = first1;
    Iterator block_start2 = first2;

    for(unsigned int i = 0; i < num_threads - 1; ++i) {
        Iterator block_end1 = block_start1;
        advance(block_end1, block_size);

        threads[i] = thread(inner_product_block<Iterator, T>(),
                          block_start1, block_end1, block_start2,
                          ref(results[i]));

        block_start1 = block_end1;
        advance(block_start2, block_size);
    }

    inner_product_block<Iterator, T>()(block_start1, last1, block_start2, results[num_threads - 1]);

    for(auto& t : threads) {
        t.join();
    }

    return accumulate(results.begin(), results.end(), init);
}


void testStandardAlgorithms(const vector<double>& vec1, const vector<double>& vec2, int iterations = 1) {
    double result;

    std::cout << "inner_product without policy: ";
    timeit([&vec1, &vec2, &result]() {
        result = inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    });

    std::cout << "transform_reduce + seq: ";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::seq,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);

    std::cout << "transform_reduce + par: ";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::par,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);

    std::cout << "transform_reduce + unseq: ";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::unseq,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);

    std::cout << "transform_reduce par_unseq: ";
    timeit([&vec1, &vec2, &result]() {
        result = std::transform_reduce(execution::par_unseq,
                                vec1.begin(), vec1.end(), vec2.begin(), 0.0);
    }, iterations);
}

void testCustomInnerProduct(const vector<double>& vec1, const vector<double>& vec2, int iterations = 1) {

    double result;
    const unsigned int hardware_threads = std::thread::hardware_concurrency();
    const unsigned int number_of_threads = hardware_threads != 0 ? hardware_threads : 2;

    for (unsigned long i = 1; i <= number_of_threads; ++i) {
        std::cout << std::format("custom algorithm with {} threads: ", i);
        timeit([&vec1, &vec2, &result, &i]() {
            result = parallel_inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0.0, i);
        }, iterations);
    }
}

int main() {

    const int ITERATIONS_NUMBER = 100;

    const std::vector<size_t> sizes = {
        1000000,
        10000000,
        100000000
    };

    try {
        for (const size_t& size : sizes ) {
            std::vector<double> vec1 = generateRandomVector(size);
            std::vector<double> vec2 = generateRandomVector(size);
            std::cout << std::format("\n===================\nSize of generated test dataset: {} \n \n", size);
            testStandardAlgorithms(vec1, vec2, ITERATIONS_NUMBER);
            testCustomInnerProduct(vec1, vec2, ITERATIONS_NUMBER);
        }
    } catch (const std::exception& e) {
        std::cout << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}