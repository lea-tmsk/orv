#include <thread>
#include <iostream>
#include <mutex>
#include <random>
#include <condition_variable>

class BarrierSync {
public: 
	BarrierSync(int val) {
		wait_for = val;
		waiting = 0;
	}

	void wait() {
		std::unique_lock<std::mutex> ul(mtx);

		waiting++;
		if (waiting == wait_for) {
			waiting = 0;
			cv.notify_all();
		} else {
			cv.wait(ul);
		}
	}


private:
	int wait_for;
	int waiting;

	std::mutex mtx;
	std::condition_variable cv;
};

BarrierSync bs(5);
std::mutex print;

void thread_print(int index) {
	std::random_device r;
	std::mt19937 generator(r());
	std::uniform_int_distribution<> uid(1, 500);
	
	print.lock();
	int sleep_time = uid(generator);
	std::cout << index << " sleep for " << sleep_time << " milliseconds \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
	print.unlock();

	bs.wait();

	print.lock();
	sleep_time = uid(generator);
	std::cout << index << " sleep for " << sleep_time << " milliseconds \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
	print.unlock();
}


int main() {
	int count = 5;
	std::vector<std::thread> threads;

	for (int i = 0; i < count; i++) {
		threads.emplace_back(thread_print, i);
	}

	for (auto& thread : threads) {
		thread.join();
	}

	return 0;
}