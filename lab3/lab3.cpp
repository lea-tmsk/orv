#include <thread>
#include <iostream>
#include <queue>
#include <shared_mutex>

class SafeThreadQueue {
public:
	SafeThreadQueue() {};
	~SafeThreadQueue() {};

	void push(int val) {
		back.lock();
		
		if (queue.size() < 2) {
			front.lock();
		}
		queue.push(val);

		if (queue.size() < 3) {
			front.unlock();
		}

		back.unlock();
	};

	void pop() {
		if (queue.empty()) {
			return;
		}
		front.lock();

		if (queue.size() == 1) {
			back.lock();
		}

		queue.pop();

		front.unlock();
		if (queue.empty()) {
			back.unlock();
		}
	};

	int get_min() {
		if (queue.empty()) {
			return NULL;
		}

		front.lock();
		back.lock();

		int min = queue.front();
		for (std::queue<int> temp = queue; !temp.empty(); temp.pop()) {
			if (temp.front() < min) {
				min = temp.front();
			}
		}

		front.unlock();
		back.unlock();
		return min;
	};


	int get_max() {
		if (queue.empty()) {
			return NULL;
		}

		front.lock();
		back.lock();

		int max = queue.front();
		for (std::queue<int> temp = queue; !temp.empty(); temp.pop()) {
			if (temp.front() > max) {
				max = temp.front();
			}
		}

		front.unlock();
		back.unlock();
		return max;
	};

	void print() {
		if (queue.empty()) {
			return;
		}

		back.lock();
		front.lock();

		std::cout << "print: ";

		for (std::queue<int> temp = queue; !temp.empty(); temp.pop()) {
			std::cout << temp.front() << " ";
		}

		std::cout << std::endl;

		front.unlock();
		back.unlock();
	}

private:
	std::queue<int> queue; //очередь
	std::shared_mutex front; //защита доступа к началу очереди
	std::shared_mutex back; //защита доступа к концу очереди
};

int main() {
	SafeThreadQueue q;

	std::thread pop1(&SafeThreadQueue::pop, &q);
	
	std::thread push1(&SafeThreadQueue::push, &q, 1);
	std::thread push2(&SafeThreadQueue::push, &q, 2);
	std::thread push3(&SafeThreadQueue::push, &q, 3);
	std::thread push4(&SafeThreadQueue::push, &q, 10);
	

	int min_val, max_val;
	
	std::thread max([&] {
		max_val = q.get_max();
	});

	std::thread print(&SafeThreadQueue::print, &q);
	std::thread pop2(&SafeThreadQueue::pop, &q);
	
	std::thread min([&] {
		min_val = q.get_min();
	});


	pop1.join();
	pop2.join();
	push1.join();
	push2.join();
	push3.join();
	push4.join();
	max.join();
	min.join();
	print.join();

	std::cout << "Max: " << max_val << std::endl;
	std::cout << "Min: " << min_val << std::endl;


	return 0;
}