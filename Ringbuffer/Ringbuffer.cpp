#include <iostream>
#include <thread>
#include "RB.h"
#include <vector>
#include <atomic>
#include <memory>
template <typename T>
void write(Ringbuffer<T>& buffer,std::mutex& lock) {
	T input;
	while  (std::cin >> input) {
		buffer.write_item(input,lock);
	}
	
}

template <typename T>
void read(Ringbuffer<T>& buffer,std::mutex& lock) {
	while (true) {
		T* read_exists{ buffer.read_item(lock) };
		if (read_exists) {
			lock.lock();
			std::cout << "The buffer read " << *read_exists << "\n";
			lock.unlock();
		}
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(2s);
		
	}
	
}

int main()
{
	std::mutex lock_rb;
	Ringbuffer<int> buffer1(5);
	std::thread t1(write<int>, std::ref(buffer1),std::ref(lock_rb));
	std::thread t2(read<int>, std::ref(buffer1),std::ref(lock_rb));
	/*
	buffer1.write_item(3);
	buffer1.write_item(6);
	buffer1.write_item(2);
	buffer1.write_item(1);
	buffer1.write_item(8);
	buffer1.write_item(9);
	std::cout << buffer1.read_item(0);
	*/
	t1.join();
	t2.join();
}