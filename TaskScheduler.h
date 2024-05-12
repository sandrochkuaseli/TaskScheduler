#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <vector>
#include "Task.h"

class TaskScheduler {
private:
	std::vector<Task> tasks;

public:

	void addTask(const Task& task);
	void removeTask(const Task& task);

	std::vector<Task> getAllTasks() const;

	void printAllTasks() const;

	void printTask(int index) const;


};

#endif // !TASKSCHEDULER_H
