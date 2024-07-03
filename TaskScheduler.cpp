#include "TaskScheduler.h"
#include "FileHandler.h"
#include <iostream>
#include <regex>

TaskScheduler::TaskScheduler(const std::string& filename) : filename(filename) {
    loadTasksFromFile();
}

void TaskScheduler::addTask(const Task& task) {
    if (checkFormat(task)) {
        tasks.push_back(task);
        tasks[tasks.size() - 1].setTaskID(tasks.size() - 1);

        for (int i : task.getDependencies()) {
            tasks[i].setDependant(task.getTaskID());
        }
    }
}

void TaskScheduler::editTask(int index, const Task& task) {
    if (index >= 0 && index < tasks.size()) {
        tasks[index] = task;
    }
    else {
        std::cout << "Invalid task index." << std::endl;
    }
}

void TaskScheduler::removeTask(int index) {
    if (index >= 0 && index < tasks.size()) {
        tasks.erase(tasks.begin() + index);
    }
    else {
        std::cout << "Invalid task index." << std::endl;
    }
}

void TaskScheduler::listTasks() const {
    if (tasks.empty()) {
        std::cout << "No tasks found." << std::endl;
    }
    else {
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << i + 1 << ". " << tasks[i].getTitle() << " - Due: " << tasks[i].getDueDate()
                << " - Priority: " << tasks[i].getPriority() << std::endl;
        }
    }
}

void TaskScheduler::exportTasks() const {
    FileHandler::exportTasks(filename, tasks);
}

void TaskScheduler::importTasks(const std::string& importFilename) {
    FileHandler::importTasks(importFilename, tasks);
}

void TaskScheduler::loadTasksFromFile() {
    FileHandler::importTasks(filename, tasks);
}

void TaskScheduler::saveTasksToFile() const {
    FileHandler::writeTasksToFile(filename, tasks);
}

void TaskScheduler::removeAllTasks() {
    tasks.clear();
}


bool TaskScheduler::dueDateValidity(const std::string& dueDateString) {
    std::regex pattern("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}");

    return std::regex_match(dueDateString, pattern);
}

bool TaskScheduler::checkFormat(const Task& task)
{
    bool valid = true;
    // Title not more than 200 char
    if (task.getTitle().length() > 200) {
        std::cout << "!! Task not added to the scheduler: Title exceeds the character limit !!" << std::endl;
        valid = false;
    }
    else if (!dueDateValidity(task.getDueDate())) {
        std::cout << "!! Task not added to the scheduler: Due date '" << task.getDueDate() << "' format is incorrect !!" << std::endl;
        valid = false;
    }
    else if (1 < task.getPriority() && task.getPriority() > 5) {
        std::cout << "!! Task not added to the scheduler: Priority out of bounds !!" << std::endl;
        valid = false;
    }

    return valid;
}

void TaskScheduler::setDependants(int taskId, int dependantId)
{
    tasks[taskId].getDependants().push_back(dependantId);
}

const std::vector<Task>& TaskScheduler::getTasks() const {
    return tasks;
}
