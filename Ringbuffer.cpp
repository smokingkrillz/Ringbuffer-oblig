#include <thread>
#include "RB.h"
//to put the thread to sleep
using namespace std::literals::chrono_literals;
//a function that takes input from the keyboard
//writes to the buffer
template <typename T>
void write(Ringbuffer<T>& buffer,std::mutex& lock,bool* stop) {
	std::cout << "Write something!\n";
	T input;
	while  (*stop==1 && std::cin >> input && input != '#') {
	buffer.write_item(input, lock);
	}
	//we are out of the while loop if someone enters a # 
	//stop turns false and program stops
	*stop = false; 
}
// another function that randomly puts input to the buffer
template <typename T>
void random_write(Ringbuffer<T>& buffer, std::mutex& lock,bool* stop) {
	int letters[]{ 0,1,2,3,4,5,6,7,8,9 };
	while (*stop) {
		lock.lock();
		auto x = letters[rand() % 9];
		lock.unlock();
		buffer.write_item(x, lock);
		std::this_thread::sleep_for(std::chrono::seconds(5));

	}
}
//function that reads from the buffer
template <typename T>
void read(Ringbuffer<T>& buffer,std::mutex& lock,bool* stop) {
	while (*stop) {
		T* read_exists{ buffer.read_item(lock) };
		//displays only if there is something to read
		if (read_exists) { 
			lock.lock();
			std::cout << "The buffer read " << *read_exists << "\n";
			lock.unlock();
		}
		std::this_thread::sleep_for(2s);
	}
}

int main()
{
	//this bool value turns to false when the user types a #, and the thread stops
	bool stop{ 1 };
	//mutex for synchronising
	std::mutex lock_rb;

	Ringbuffer<int> buffer1(5);
	//write thread, takes input from keyboard
	std::thread keyboard_write_t(write<int>, std::ref(buffer1),std::ref(lock_rb),&stop);
	//write thread, takes random numbers and writes to the buffer
	std::thread random_write_t(read<int>, std::ref(buffer1),std::ref(lock_rb), &stop);
	//read thread, reads from buffer every 2 seconds
	std::thread read_t(random_write<int>, std::ref(buffer1),std::ref(lock_rb),&stop);

	keyboard_write_t.join();
	random_write_t.join();
	read_t.join();
}