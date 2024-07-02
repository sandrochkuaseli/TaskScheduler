#include "FileHandler.h"
#include "TaskScheduler.h"
#include <fstream>
#include <iostream>
#include <sstream>

void FileHandler::exportTasks(const std::string& filename, const std::vector<Task>& tasks) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& task : tasks) {
            task.saveToFile(file);
        }
        file.close();
        std::cout << "Tasks exported to " << filename << " successfully." << std::endl;
    }
    else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}

void FileHandler::importTasks(const std::string& filename, std::vector<Task>& tasks) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::string title = line;

            std::string description;
            std::getline(file, description);

            std::string dueDate;
            std::getline(file, dueDate);

            int priority;
            std::string priorityStr;
            std::getline(file, priorityStr);
            std::istringstream priorityStream(priorityStr);
            priorityStream >> priority;

            std::cout << "Priority: " << priority << std::endl;

            bool recurring;
            std::string recurringStr;
            std::getline(file, recurringStr);
            std::istringstream recurringStream(recurringStr);
            recurringStream >> recurring;

            Task task(title, description, dueDate, priority, recurring);
            if (TaskScheduler::checkFormat(task)) {
                tasks.push_back(task);
            }

            while (std::getline(file, line)) {
                if (line == "---") {
                    break;
                }
            }
        }

        file.close();
        std::cout << "Tasks imported from " << filename << " successfully." << std::endl;
    }
    else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}



void FileHandler::writeTasksToFile(const std::string& filename, const std::vector<Task>& tasks) {
    std::ofstream file(filename, std::ios::trunc);
    if (file.is_open()) {
        for (const auto& task : tasks) {
            file << task.getTitle()<< '\n';
            file << task.getDescription() << '\n';
            file << task.getDueDate() << '\n';
            file << task.getPriority() << '\n';
            file << task.isRecurring() << '\n';
            file << "---" << std::endl;
        }
    }
    else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
    file.close();
}
