#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "Task.h"
#include <vector>
#include <string>


/**
  * Main class that combines most of the other classes functionalities.
  * Handles almost every action with tasks.
  */
class TaskScheduler {
private:
    std::vector<Task> tasks;
    std::string filename;

public:
    TaskScheduler(const std::string& filename);

    void addTask(const Task& task);
    void editTask(int index, std::string attribute, std::string newAttributeDefinition);
    void removeTask(int index);
    void listTasks(std::vector<Task> taskVec) const;
    void listTasksByPriority() const;
    void showTask(int index);
    void exportTasks(std::string filename) const;
    void importTasks(const std::string& importFilename);
    void loadTasksFromFile();
    void removeAllTasks();
    static bool checkFormat(const Task& task);
    static bool dueDateValidity(const std::string& dueDateString);

    void setDependants(int taskId, int dependantId);
    const std::vector<Task>& getTasks() const;
    std::vector<Task>& getTasks();

    void setComplete(int taskId, bool complete);

    Task& operator[](size_t index);
    const Task& operator[](size_t index) const;

    static int getDaysInMonth(int year, int month) {
        // Checks for leap year
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                return 29;
            }
            else {
                return 28;
            }
        }

        switch (month)
        {
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            break;
        }

        return 31;
    }
};

#endif // TASK_SCHEDULER_H
