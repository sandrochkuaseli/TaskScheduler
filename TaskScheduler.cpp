#include "TaskScheduler.h"
#include "FileHandler.h"
#include <iostream>

TaskScheduler::TaskScheduler(const std::string& filename) : filename(filename) {
    loadTasksFromFile();
}

void TaskScheduler::addTask(const Task& task) {
    tasks.push_back(task);
    saveTasksToFile();
}

void TaskScheduler::editTask(int index, const Task& task) {
    if (index >= 0 && index < tasks.size()) {
        tasks[index] = task;
        saveTasksToFile();
    }
    else {
        std::cout << "Invalid task index." << std::endl;
    }
}

void TaskScheduler::removeTask(int index) {
    if (index >= 0 && index < tasks.size()) {
        tasks.erase(tasks.begin() + index);
        saveTasksToFile();
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
    tasks.clear();
    FileHandler::importTasks(importFilename, tasks);
}

void TaskScheduler::loadTasksFromFile() {
    FileHandler::readTasksFromFile(filename, tasks);
}

void TaskScheduler::saveTasksToFile() const {
    FileHandler::writeTasksToFile(filename, tasks);
}

std::vector<Task> TaskScheduler::getTasks() {
    return tasks;
}
