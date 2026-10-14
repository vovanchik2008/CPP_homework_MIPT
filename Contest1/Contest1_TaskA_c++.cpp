#include <iostream>
#include <iomanip>

struct MinMax {
  int min;
  int max;
};

double AverageTemperature(const int* temperatures, const int n) {
  double summ = 0;
  for (int i = 0; i < n; ++i) {
    summ += *(temperatures + i);
  }
  return summ / n;
}

MinMax LowestAndHighest(const int* temperatures, const int n) {
  MinMax min_max;
  min_max.min = temperatures[0];
  min_max.max = temperatures[0];
  for (int i = 1; i < n; ++i) {
    if (*(temperatures + i) < min_max.min) {
      min_max.min = *(temperatures + i);
    }
    if (*(temperatures + i) > min_max.max) {
      min_max.max = *(temperatures + i);
    }
  }
  return min_max;
}

int FrostDays(const int* temperatures, const int n) {
  int frost_counter = 0;
  for (int i = 0; i < n; ++i) {
    if (*(temperatures + i) < 0) {
      ++frost_counter;
    }
  }
  return frost_counter;
}

int main() {
  int n;
  std::cin >> n;
  int* temperatures = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> *(temperatures + i);
  }
  std::cout << std::fixed << std::setprecision(1) << AverageTemperature(temperatures, n) << "\n";
  MinMax min_max = LowestAndHighest(temperatures, n);
  std::cout << min_max.min << " " << min_max.max << "\n";
  std::cout << FrostDays(temperatures, n);
  delete[] temperatures;
  return 0;
}