#pragma once
#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
template <typename T>
class Ringbuffer
{
private:
	//the buffer is the vector with the objects
	std::vector<T> m_Objects;
	std::atomic<size_t> m_read; //tail
	std::atomic<size_t> m_write; //head
public:
	explicit Ringbuffer(size_t size) :
		m_Objects(size), m_read{ 0 }, m_write{ 0 } {} 

	auto isEmpty() const {
		return m_read == m_write;
	}
	auto isFull() const {
		return((m_write > m_read) &&
			(m_write % m_Objects.size() == m_read % m_Objects.size()));
	}
	void write_item(T& input,std::mutex& lock) {
		if(isFull() == false){ //as long as buffer is not full we can write
			lock.lock();
			m_Objects[m_write] = input;
			m_write++;
			if (m_write == m_Objects.size()) m_write = 0; //resetting index
			lock.unlock();
		}
	}
	T* read_item(std::mutex& lock) {
		if (!isEmpty()) { //reads only if it isn't empty
			lock.lock();
			if (m_read == m_Objects.size()) m_read = 0;
			lock.unlock();
			auto return_obj = &(m_Objects[m_read]);
			m_read++;
			return return_obj;
		}
		else return nullptr;
	}

};

