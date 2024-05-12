#include "TaskScheduler.h"
#include <iostream>

void TaskScheduler::addTask(const Task& task)
{
	tasks.push_back(task);
}

void TaskScheduler::removeTask(const Task& task)
{
	for (auto it = tasks.begin(); it != tasks.end(); ++it){
		if (*it == task) {
			tasks.erase(it);
			break;
		}
	}
}

std::vector<Task> TaskScheduler::getAllTasks() const
{
	return tasks;
}

void TaskScheduler::printAllTasks() const
{
	std::cout << "Displaying all the tasks in Scheduler:" << std::endl;
	for (const auto& task : tasks) {
		std::cout << "Title: " << task.getTitle() << ", Description: " << task.getDescription()
			<< ", Due Date : " << task.getDueDate() << ", Priority: " << task.getPriority()
			<< ", Recurring: " << (task.isRecurring() ? "YES" : "NO") << std::endl;
	}
}

void TaskScheduler::printTask(int index) const
{
	if (index >= 0 && index < tasks.size()) {
		const Task& task = tasks[index];

		std::cout << "Title: " << task.getTitle() << ", Description: " << task.getDescription()
			<< ", Due Date : " << task.getDueDate() << ", Priority: " << task.getPriority()
			<< ", Recurring: " << (task.isRecurring() ? "YES" : "NO") << std::endl;

	}
	else {
		std::cout << "Invalid task index!" << std::endl;
	}
}






