#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <cmath>

size_t threshold = 16;

class ArrayGenerator {
  std::random_device rand_dev_;

 public:
  std::vector<int> ArrayFromRange(int size, int from, int to) {
    std::vector<int> result(size);

    std::uniform_int_distribution<int> distribution(from, to);
    for(size_t i = 0; i < size; ++i) {
      result[i] = distribution(rand_dev_);
    }

    return result;
  }

  std::vector<int> ReversedArray(int size, int from, int to) {
    std::vector<int> result(size);

    std::uniform_int_distribution<int> distribution(from, to);
    for(size_t i = 0; i < size; ++i) {
      result[i] = distribution(rand_dev_);
    }

    std::sort(result.begin(), result.end(), std::greater<int>());
    return result;
  }

  std::vector<int> AlmostSortedArray(int size, int from, int to, int unsortedCount) {
    std::vector<int> result(size);

    std::uniform_int_distribution<int> distribution(from, to);
    for(size_t i = 0; i < size; ++i) {
      result[i] = distribution(rand_dev_);
    }

    std::sort(result.begin(), result.end());

    for(int i = 0; i < unsortedCount; ++i) {
      std::swap(result[rand_dev_() % size], result[rand_dev_() % size]);
    }

    return result;
  }
};

void quickSortImpl(std::vector<int>& a, int begin, int end) {
  std::random_device rand_dev;
  std::uniform_int_distribution<int> distribution(begin, end-1);
  int pivot = a[distribution(rand_dev)];
  int i = begin;
  int j = end - 1;

  while(i <= j){
    while(a[i] < pivot){
      ++i;
    }
    while(a[j] > pivot){
      --j;
    }
    if (i <= j){
      std::swap(a[i], a[j]);
      ++i;
      --j;
    }
  }

  if (begin < j){
    quickSortImpl(a, begin, j + 1);
  }
  if (i < end){
    quickSortImpl(a, i, end);
  }
}

void quickSort(std::vector<int>& a){
  if (a.size() < 2){
      return;
  }

  quickSortImpl(a, 0, a.size());
}

void heapify(std::vector<int>& a, int n, int i, int begin){
  int largest = i;
  int left = 2 * (i - begin) + 1 + begin;
  int right = 2 * (i - begin) + 2 + begin;

  if (left < begin + n && a[left] > a[largest]) {
    largest = left;
  }

  if (right < begin + n && a[right] > a[largest]) {
    largest = right;
  }

  if (largest != i) {
    std::swap(a[i], a[largest]);
    heapify(a, n, largest, begin);
  }
}

void heapSort(std::vector<int>& a, int begin, int end){
  int n = end - begin;
  for (int i = begin + n / 2 - 1; i >= begin; --i){
    heapify(a, n, i, begin);
  }
  for (int i = end - 1; i > begin; --i){
    std::swap(a[begin], a[i]);
    heapify(a, i - begin, begin, begin);
  }
}

void insertionSort(std::vector<int>& a, int begin, int end){
  for (int i = begin + 1; i < end; i++){
    int key = a[i];
    int j = i - 1;
    while(j >= begin && a[j] > key){
      a[j + 1] = a[j];
      --j;
    }
    a[j + 1] = key;
  }
}

void introSortImpl(std::vector<int>& a, int begin, int end, int maxDepth){
  if (end - begin < threshold){
    insertionSort(a, begin, end);
    return;
  }

  if (maxDepth == 0){
    heapSort(a, begin, end);
    return;
  }

  std::random_device rand_dev;
  std::uniform_int_distribution<int> distribution(begin, end-1);
  int pivot = a[distribution(rand_dev)];
  int i = begin;
  int j = end - 1;

  while(i <= j){
    while(a[i] < pivot){
      ++i;
    }
    while(a[j] > pivot){
      --j;
    }
    if (i <= j){
      std::swap(a[i], a[j]);
      ++i;
      --j;
    }
  }

  if (begin < j){
    introSortImpl(a, begin, j + 1, maxDepth - 1);
  }
  if (i < end){
    introSortImpl(a, i, end, maxDepth - 1);
  }
}

void introSort(std::vector<int>& a){
  if (a.size() < 2){
    return;
  }

  int maxDepth = 2 * std::log2(a.size());
  introSortImpl(a, 0, a.size(), maxDepth);
}

template<typename T>
void PrintArrayIntoFile(std::ofstream& file, const std::vector<T>& array) {
  for (size_t i = 0; i < array.size(); ++i) {
    file << array[i];
    if (i < array.size() - 1) {
      file << ", ";
    }
  }
  file << '\n';
}


class SortTester {
 public:
  template<class SortFunc>
  double StopWatchSortFunction(std::vector<int>& array, int timesOfComputations, SortFunc sortFunction){
    double result = 0;
    for(size_t i = 0; i < timesOfComputations; ++i){
      auto start = std::chrono::high_resolution_clock::now();
      sortFunction(array);
      auto end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double, std::milli> duration = end - start;
      result += duration.count();
    }

    return result/timesOfComputations;
  }

  template<class SortFunc>
  void Test(SortFunc sortFunction, const std::string& fileName){
    ArrayGenerator arrGenerator;

    std::vector<int> rangeArray = arrGenerator.ArrayFromRange(10000, 0, 6000);
    std::vector<int> reversedArray = arrGenerator.ReversedArray(10000, 0, 6000);
    std::vector<int> almostSortedArray = arrGenerator.AlmostSortedArray(10000, 0, 6000, 10);

    std::vector<size_t> arraySizes;
    std::vector<double> rangeArrayDurations;
    std::vector<double> reversedArrayDurations;
    std::vector<double> almostSortedArrayDurations;
    SortTester sortTester;
    for (size_t size = 500; size <= 10000; size += 100) {
      std::vector<int> rangeSubArray(rangeArray.begin(), rangeArray.begin() + size);
      std::vector<int> reversedSubArray(reversedArray.begin(), reversedArray.begin() + size);
      std::vector<int> almostSortedSubArray(almostSortedArray.begin(), almostSortedArray.begin() + size);

      double rangeArrayTime = sortTester.StopWatchSortFunction(rangeSubArray, 100, sortFunction);
      double reversedArrayTime = sortTester.StopWatchSortFunction(reversedSubArray, 100, sortFunction);
      double almostSortedArrayTime = sortTester.StopWatchSortFunction(almostSortedSubArray, 100, sortFunction);

      arraySizes.push_back(size);
      rangeArrayDurations.push_back(rangeArrayTime);
      reversedArrayDurations.push_back(reversedArrayTime);
      almostSortedArrayDurations.push_back(almostSortedArrayTime);
    }

    WriteResultsToFile(fileName, arraySizes, rangeArrayDurations, reversedArrayDurations, almostSortedArrayDurations);
  }

  void WriteResultsToFile(const std::string& fileName, std::vector<size_t>& arraySizes, std::vector<double>& rangeArrayDurations, std::vector<double>& reversedArrayDurations, std::vector<double>& almostSortedArrayDurations) {
    std::ofstream file(fileName);
    PrintArrayIntoFile(file, arraySizes);
    PrintArrayIntoFile(file, rangeArrayDurations);
    PrintArrayIntoFile(file, reversedArrayDurations);
    PrintArrayIntoFile(file, almostSortedArrayDurations);
    file.close();
  }
};

int main() {
  SortTester sort_tester;
  sort_tester.Test(quickSort, "../A3/quickSort.txt");
  sort_tester.Test(introSort, "../A3/introSort.txt");

  for(; threshold <= 16384; threshold *= 2)
    sort_tester.Test(introSort, "../A3/introSort" + std::to_string(threshold) + ".txt");
  return 0;
}