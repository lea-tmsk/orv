#include <thread>
#include <iostream>
#include <vector>
#include <random>
#include <math.h>

void fillArray(int *vector, int start, int end) {
	std::random_device r;
	std::mt19937 generator(r());
	std::uniform_int_distribution<> uid(0, 200);

	for (int i = start; i < end; i++) {
		vector[i] = uid(generator);
	}
}

int main() {
	int n = 104, k = 4;
	int* nums = new int[n];

	std::vector<std::thread> threads;

	int index = 0;
	int segment_size = std::ceil((float)n / k); //������ ��������

	for (int i = 0; i < k; i++) {
		int start = i * segment_size; //������ ������� 
		int end = i < k - 1 ? start + segment_size : n; //����� ������� ��� �������, ���� ��������� �������

		threads.emplace_back(fillArray, nums, start, end);
	}

	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << "Result: \n\n";

	for (int i = 0; i < n; i++) {
		std::cout << nums[i] << " ";
	}

	std::cout << "\n\n";
	delete[] nums;

	return 0;
}