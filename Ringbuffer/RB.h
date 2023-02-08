#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <mutex>
#include <thread>
template <typename T>
class Ringbuffer
{
private:
	std::vector<T> m_Objects;
	size_t m_read; //tail
	size_t m_write; //head
public:
	explicit Ringbuffer(size_t size) :
		m_Objects(size), m_read{ 0 }, m_write{ 0 } {}

	auto get_size() {
		return m_Objects.size();
	}
	auto isEmpty() const {
		return m_read == m_write;
	}
	auto isFull() const {
		return((m_write > m_read) && (m_write % m_Objects.size() == m_read % m_Objects.size()));
	}
	void write_item(T& input,std::mutex& lock) {
		if(isFull() == false){
			// 
			lock.lock();
			m_Objects[m_write++] = input;
			if (m_write == m_Objects.size()) m_write = 0;
			lock.unlock();
		}
	}


	T* read_item(std::mutex& lock) {
		if (!isEmpty()) {
			//if (index > m_Objects.size()) std::cout << "index is bigger than size\n";
			//  t1 og T
			lock.lock();
			if (m_read == m_Objects.size()) m_read = 0;
			lock.unlock();
			// tråd 1 
			return &(m_Objects[m_read++]);
		}
		else return nullptr;
	}

};

