#include <iostream>
#include <omp.h>
#include <vector>

int main() {
	int n = 10000;

	std::vector<int> x(n, 2);
	std::vector<int> y(n, 3);

	int result = 0;

	#pragma omp parallel for reduction(+:result)
	for (int i = 0; i < n; i++) {
		result += x[i] * y[i];
	}

	std::cout << "result: " << result;

	return 0;
}