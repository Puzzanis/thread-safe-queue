#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "include/ThreadPool.h"

std::mutex countLocker;
typedef void (*FuncType) ();

void task1()
{
	std::unique_lock<std::mutex> lock(countLocker);
	std::cout << "FUNCTION " << __FUNCTION__ << " была выполнена thread_id " << std::this_thread::get_id() << std::endl;
}

void task2()
{
	std::unique_lock<std::mutex> lock(countLocker);
	std::cout << "FUNCTION " << __FUNCTION__ << " была выполнена thread_id " << std::this_thread::get_id() << std::endl;
}

void task3()
{
	std::unique_lock<std::mutex> lock(countLocker);
	std::cout << "FUNCTION " << __FUNCTION__ << " была выполнена thread_id " << std::this_thread::get_id() << std::endl;
}

void produce(RequestHandler &rh) {
	int NTask = 10;
	std::vector<FuncType> vectorOfFunctions{ task1, task2, task3 };

    for (;;) {
		if (NTask < 1) break;

		std::this_thread::sleep_for(std::chrono::seconds(1));
		rh.push_task(vectorOfFunctions);
		NTask -= 1;
    }

}

int main()
{
	setlocale(0, "");

	RequestHandler rh;
	produce(rh);

	return 0;
}