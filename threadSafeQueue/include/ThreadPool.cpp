#include "ThreadPool.h"

void ThreadPool::start()
{
	m_work = true;
	for (int i = 0; i < countCore; ++i)
	{
		m_threads.emplace_back(std::thread(&ThreadPool::work, this));
	}
}
void ThreadPool::stop()
{
	m_work = false;
	m_event_holder.notify_all();
	for (auto& t : m_threads)
	{
		t.join();
	}
}
void ThreadPool::submit(std::vector<FuncType> vec_f)
{
	std::lock_guard<std::mutex> l(m_locker);
	for (int i = 0; i < vec_f.size(); ++i)
	{
		task_type new_task([=]() { vec_f[i](); });
		m_task_queue.push(new_task);
	}

	m_event_holder.notify_one();
}
void ThreadPool::work()
{
	for (;;)
	{
		task_type task_to_do;
		{
			std::unique_lock<std::mutex> l(m_locker);
			if (m_task_queue.empty() && !m_work)  //условие завершения потока
			{
				return;
			}
			if (m_task_queue.empty())
			{
				m_event_holder.wait(l, [&]() {return !m_task_queue.empty() || !m_work; }); //засыпаем и защищаемся от wake up
			}
			if (!m_task_queue.empty())
			{
				m_task_queue.front_and_pop(task_to_do);
			}
		}
		if (task_to_do)
		{
			task_to_do();
		}
	}
}

RequestHandler::RequestHandler()
{
	m_tpool.start();
}
RequestHandler::~RequestHandler()
{
	m_tpool.stop();
}
void RequestHandler::push_task(std::vector<FuncType> vec_f)
{
	m_tpool.submit(vec_f);
}