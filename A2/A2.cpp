#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

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

void mergeSort(std::vector<int>& a){
  if (a.size() <= 1){
    return;
  }

  std::vector<int> left(a.begin(), a.begin() + a.size()/2);
  std::vector<int> right(a.begin() + a.size()/2, a.end());

  mergeSort(left);
  mergeSort(right);

  int l = 0;
  int r = 0;
  int i = 0;

  while(l < left.size() && r < right.size()){
    if (left[l] < right[r]){
      a[i++] = left[l++];
    } else {
      a[i++] = right[r++];
    }
  }

  while(l < left.size()){
    a[i++] = left[l++];
  }

  while(r < right.size()){
    a[i++] = right[r++];
  }
}

void insertionSort(std::vector<int>& a){
  for (int i = 1; i < a.size(); i++){
    int key = a[i];

    int j = i - 1;
    while(j >= 0 && a[j] > key){
        a[j + 1] = a[j];
        --j;
      }

    a[j + 1] = key;
  }
}

void mergeInsertionSort(std::vector<int>& a){
    if (a.size() <= threshold){
        insertionSort(a);
        return;
    }

    std::vector<int> left(a.begin(), a.begin() + a.size()/2);
    std::vector<int> right(a.begin() + a.size()/2, a.end());

    mergeInsertionSort(left);
    mergeInsertionSort(right);

    int l = 0;
    int r = 0;
    int i = 0;

    while(l < left.size() && r < right.size()){
        if (left[l] < right[r]){
        a[i++] = left[l++];
        } else {
        a[i++] = right[r++];
        }
    }

    while(l < left.size()){
        a[i++] = left[l++];
    }

    while(r < right.size()){
        a[i++] = right[r++];
    }
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
  sort_tester.Test(mergeSort, "../A2/mergeSort.txt");
  for(; threshold <= 16384; threshold *= 2)
    sort_tester.Test(mergeInsertionSort, "../A2/mergeInsertionSort" + std::to_string(threshold) + ".txt");


  return 0;
}