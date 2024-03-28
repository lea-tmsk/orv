#include <iostream>
#include <random>
#include <ctime>
#include <omp.h>

using namespace std;

int partition(std::vector<double> arr, int start, int end) {

    int pivot = arr[start];

    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }

    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    // Sorting left and right parts of the pivot element
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i] <= pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}

void quickSort(std::vector<double> arr, int start, int end) {

    // base case
    if (start >= end)
        return;

    // partitioning the array
    int p = partition(arr, start, end);

    // Sorting the left part
    quickSort(arr, start, p - 1);

    // Sorting the right part
    quickSort(arr, p + 1, end);
}

void quickSortParallel(std::vector<double> arr, int start, int end) {

    // base case
    if (start >= end)
        return;

    // partitioning the array
    int p = partition(arr, start, end);

    #pragma omp parallel sections 
    {
        // Sorting the left part
        #pragma omp section
        quickSortParallel(arr, start, p - 1);

        // Sorting the right part
        #pragma omp section
        quickSortParallel(arr, p + 1, end);
    }
}

int main() {
    srand(time(nullptr));
    int n = 50000;
    std::vector<double> nums(n);
    std::vector<double> nums_p(n);

    for (int i = 0; i < n; i++) {
        int num = rand() / 100.0;
        nums[i] = num;
        nums_p[i] = num;
    }

    clock_t start_time = clock();

    quickSort(nums, 0, n - 1);

    clock_t end_time = clock();

    double duration = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "usual time: " << duration << " sec" << std::endl;
    nums.resize(0);

    start_time = clock();

    quickSortParallel(nums_p, 0, n - 1);

    end_time = clock();

    duration = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "parallel time: " << duration << " sec" << std::endl;

    return 0;
}