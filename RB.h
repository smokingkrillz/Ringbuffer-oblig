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
	std::atomic<size_t> m_write;//head
	std::mutex mutex_lock;
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
	void write_item(T& input) {
		std::lock_guard<std::mutex> lock(mutex_lock);
		if(isFull() == false){ //as long as buffer is not full we can write
			m_Objects[m_write] = input;
			m_write++;
			if (m_write == m_Objects.size()) m_write = 0; //resetting index
		}
	}

	bool read_item(T* object) {
		std::lock_guard<std::mutex> lock(mutex_lock);
		if (!isEmpty()) { //reads only if it isn't empty
			if (m_read == m_Objects.size()) m_read = 0;
			*object = m_Objects[m_read];
			m_read++;
			return true;
		}
		else return false;
	}

};

