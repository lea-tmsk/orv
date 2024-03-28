#include <iostream>
#include <omp.h>
#include <vector>
#include <ctime>

void multiply_static(int n, const std::vector<std::vector<int>>& x, const std::vector<std::vector<int>>& y, std::vector<std::vector<int>>& result, int threads) {
#pragma omp parallel for schedule(static, threads)
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			for (int k = 0; k < n; ++k) {
				result[i][j] += x[i][k] * y[k][j];
			}
		}
	}
}

void multiply_dynamic(int n, const std::vector<std::vector<int>>& x, const std::vector<std::vector<int>>& y, std::vector<std::vector<int>>& result, int threads) {
#pragma omp parallel for schedule(dynamic, threads)
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			for (int k = 0; k < n; ++k) {
				result[i][j] += x[i][k] * y[k][j];
			}
		}
	}
}

void multiply_guided(int n, const std::vector<std::vector<int>>& x, const std::vector<std::vector<int>>& y, std::vector<std::vector<int>>& result, int threads) {
#pragma omp parallel for schedule(guided, threads)
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			for (int k = 0; k < n; ++k) {
				result[i][j] += x[i][k] * y[k][j];
			}
		}
	}
}

int main() {
	int n = 200;

	std::vector<std::vector<int>> x(n, std::vector<int>(n, 2));
	std::vector<std::vector<int>> y(n, std::vector<int>(n, 4));

	std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));

	int chunks = 100;
	clock_t start_time = clock();
	multiply_static(n, x, y, result, chunks);
	clock_t end_time = clock();
	double duration = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
	std::cout << "static, " << chunks << " chunks, time: " << duration << " sec" << std::endl;


	start_time = clock();
	multiply_dynamic(n, x, y, result, chunks);
	end_time = clock();
	duration = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
	std::cout << "dynamic, " << chunks << " chunks, time: " << duration << " sec" << std::endl;
	
	
	start_time = clock();
	multiply_guided(n, x, y, result, chunks);
	end_time = clock();
	duration = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
	std::cout << "guided, " << chunks << " chunks, time: " << duration << " sec" << std::endl;	

	return 0;
}