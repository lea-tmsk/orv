#include <thread>
#include <iostream>
#include <chrono>

void countdown(int n) {
	for (auto i = n; i >= 0; i--) {
		std::cout << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
};

int main() {
	const int n = 10;
	
	std::thread tr1(countdown, n);
	std::thread tr2(countdown, n);
	
	tr1.join();
	tr2.join();

	return 0;
}