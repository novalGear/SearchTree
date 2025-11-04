#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>

#include "os_tree.hpp"

template <typename T>
int count_in_range_set(const std::set<T>& s, T fst, T snd) {
    auto start_it = s.lower_bound(fst);
    auto end_it = s.upper_bound(snd);
    int dist = std::distance(start_it, end_it);
    return (dist > 0) ? dist : 0;
}

int main() {
    const int N = 10000;
    const int M = 100000;
    const int NUM_RUNS = 5;

    std::set<int> my_set;
    for (int i = 0; i < N; ++i) {
        my_set.insert(i);
    }

    OS_Tree::SearchTree my_tree;
    for (int i = 0; i < N; ++i) {
        my_tree.insert(i);
    }

    std::vector<std::pair<int, int>> range_queries(M);
    const unsigned int seed = 12345;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, N - 1);

    for (int i = 0; i < M; ++i) {
        int fst = dis(gen);
        int snd = dis(gen);
        if (fst > snd) std::swap(fst, snd);
        range_queries[i] = {fst, snd};
    }

    std::cout << "Starting " << NUM_RUNS << " runs for std::set with N=" << N << ", M=" << M << ".\n";
    std::cout << "\n--- std::set Results ---\n";
    for (int run = 0; run < NUM_RUNS; ++run) {
        auto start = std::chrono::high_resolution_clock::now();
        long long total_count_set = 0;
        for (const auto& [fst, snd] : range_queries) {
            total_count_set += count_in_range_set(my_set, fst, snd);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Run " << (run + 1) << ": Total found " << total_count_set << " items in " << duration.count() << " microseconds.\n";
    }

    std::cout << "\n--- OS_Tree Results ---\n";
    for (int run = 0; run < NUM_RUNS; ++run) {
        auto start = std::chrono::high_resolution_clock::now();
        long long total_count_tree = 0;
        for (const auto& [fst, snd] : range_queries) {
            total_count_tree += my_tree.count_in_range(fst, snd);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Run " << (run + 1) << ": Total found " << total_count_tree << " items in " << duration.count() << " microseconds.\n";
    }

    return 0;
}
