#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "Task.h"
#include <vector>
#include <string>

class TaskScheduler {
private:
    std::vector<Task> tasks;
    std::string filename;

public:
    TaskScheduler(const std::string& filename);

    void addTask(const Task& task);
    void editTask(int index, std::string attribute, std::string newAttributeDefinition);
    void removeTask(int index);
    void listTasks() const;
    void showTask(int index);
    void exportTasks() const;
    void importTasks(const std::string& importFilename);
    void loadTasksFromFile();
    void saveTasksToFile() const;
    void removeAllTasks();
    static bool checkFormat(const Task& task);
    static bool dueDateValidity(const std::string& dueDateString);

    void setDependants(int taskId, int dependantId);
    const std::vector<Task>& getTasks() const;

    void setComplete(int taskId, bool complete);

    Task& operator[](size_t index);
    const Task& operator[](size_t index) const;
};

#endif // TASK_SCHEDULER_H
