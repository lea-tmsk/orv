#include <thread>
#include <vector>
#include <iostream>
#include <random>
#include <omp.h>

class Task {
public:
	Task(int val) {
		int num = std::rand();
		if (num % 2 == 0) {
			this->func = &Task::wait_for_timeout;
		}
		else {
			this->func = &Task::print_numbers;
		}
		timeout = val;
	}

	void wait_for_timeout() {
		std::cout << std::endl << std::this_thread::get_id() << " sleep for: " << timeout << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
		std::cout << std::endl << std::this_thread::get_id() << " end of sleep: " << timeout << std::endl;
	}

	void print_numbers() {
		std::cout << std::endl << std::this_thread::get_id() << " print_numbers: \n";
		for (int i = 0; i < 10; i++) {
			std::cout << i << " ";
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::cout << std::endl;
	}

	void call_function() {
		(this->*func) ();
	}

	int timeout;
	void (Task::*func)();
};

void tasks_manager(std::vector<Task> tasks, int n) {
	#pragma omp parallel for num_threads(n)
	for (int i = 0; i < tasks.size(); i++) {
		tasks[i].call_function();
	}
}


int main() {
	std::srand(std::time(nullptr));

	int n = 5; //количество потоков
	int k = 20; //количество задач

	std::vector<Task> tasks;
	std::random_device r;
	std::mt19937 generator(r());
	std::uniform_int_distribution<> uid(500, 2000);

	for (int i = 0; i < k; i++) {
		tasks.push_back(Task(uid(generator)));
	}

	tasks_manager(tasks, n);

	return 0;
}