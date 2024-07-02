#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "Task.h"
#include <vector>

class TaskScheduler {
private:
    std::vector<Task> tasks;
    std::string filename;

public:
    TaskScheduler(const std::string& filename);

    void addTask(const Task& task);
    void editTask(int index, const Task& task);
    void removeTask(int index);
    void listTasks() const;
    void exportTasks() const;
    void importTasks(const std::string& importFilename);
    void loadTasksFromFile();
    void saveTasksToFile() const;

    std::vector<Task> getTasks();
};

#endif // TASK_SCHEDULER_H
