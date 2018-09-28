#include <iostream>
#include <string>
#include <random>
#include <algorithm>

#include "benchmark/benchmark.h"

#include "resource.h"

/* How long it takes to create a string. */
static void BM_CreateString(benchmark::State& state) {
  for(auto _ : state) {
    Resource<std::string>::Create("arial.ttf");
  }
}

/* Push one element to random location on our resource. */
static void PushElementInRandomPos(benchmark::State& state) {
  auto& c = Resource<std::string>::GetContainer();
  size_t sz = c.size();
  std::random_device rd;  // random number from hardware
  static std::mt19937 rng(rd()); //seed
  std::uniform_int_distribution<size_t> distr(0, sz - 1); // define the range
  size_t i = distr(rng);
  c.at(i) = std::make_pair(std::string("arial.ttf"), std::string());
}

/* Since we use static container, we must clear it. Not measured. */
static void ClearContainer(benchmark::State& state) {
  for (auto _ : state) {
    auto& c = Resource<std::string>::GetContainer();
    c.clear();
  }
}

/* Get string by hardcoded name. Not measured. */
static void StringByName(benchmark::State& state) {
  std::string str;
  benchmark::DoNotOptimize(&str);
  str = Resource<std::string>::GetByName("arial.ttf");
  benchmark::ClobberMemory();
}

/* Create many strings as a resource. Not measured. */
static void CreateManyStrings(benchmark::State& state) {
  std::vector<std::string> vec(size_t(state.range(0)), "random-string");
  for(const auto& str : vec) {
    benchmark::DoNotOptimize(&str);
    Resource<std::string>::Create(str);
    benchmark::ClobberMemory();
  }
}

/* Push element in random position and find by name. */
static void BM_CreateManyStringsAndFindByName(benchmark::State& state) {
  CreateManyStrings(state);
  PushElementInRandomPos(state);
  for(auto _ : state) {
    StringByName(state); // measure only time to find element
  }
  ClearContainer(state);
}

BENCHMARK(BM_CreateString);

BENCHMARK(BM_CreateManyStringsAndFindByName)->DenseRange(3, 20, 5)->Repetitions(3)->DisplayAggregatesOnly(true);
BENCHMARK(BM_CreateManyStringsAndFindByName)->DenseRange(20, 100, 20)->Repetitions(3)->DisplayAggregatesOnly(true);

BENCHMARK_MAIN();
