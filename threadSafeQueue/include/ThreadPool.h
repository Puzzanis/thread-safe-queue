#pragma once

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

#include "Queue.h"

typedef std::function<void()> task_type;
typedef void (*FuncType) ();

class ThreadPool
{
public:
	void start();
	void stop();
	void submit(std::vector<FuncType> vec_f);
	void work();

private:
	int countCore{ static_cast<int>(std::thread::hardware_concurrency()) };
	std::vector<std::thread> m_threads;
	SafeQueue<task_type> m_task_queue;
	std::mutex m_locker;
	std::condition_variable m_event_holder;
	volatile bool m_work;
};

class RequestHandler
{
private:
	ThreadPool m_tpool;

public:
	RequestHandler();
	~RequestHandler();
	void push_task(std::vector<FuncType> vec_f);
};