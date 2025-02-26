#pragma once

#include <iostream>
#include <mutex>
#include <queue>

template <class T>
class SafeQueue
{
	std::queue<T> Buffer;
	std::mutex mtx;
	std::condition_variable cond;

public:
	SafeQueue() {}

	void push(T Data)
	{
		std::unique_lock<std::mutex> lock(mtx);
		Buffer.push(std::move(Data));
		cond.notify_one();
	}

	void front_and_pop(T &Data)
	{
		std::unique_lock<std::mutex> lock(mtx);
		cond.wait(lock, [this]() { return !Buffer.empty(); });
		Data = std::move(Buffer.front());
		if (!Buffer.empty())
		{
			Buffer.pop();
		}
		
	}
	
	bool empty() 
	{ 
		std::unique_lock<std::mutex> lock(mtx);
		return Buffer.empty();  
	}
};