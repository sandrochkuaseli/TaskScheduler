#include "FileHandler.h"
#include <fstream>
#include <iostream>

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
        std::string title, description, dueDate;
        int priority;
        bool recurring;

        while (std::getline(file, title)) {
            std::getline(file, description);
            std::getline(file, dueDate);
            file >> priority;
            file.ignore();
            file >> std::boolalpha >> recurring;
            file.ignore();

            Task task(title, description, dueDate, priority, recurring);
            tasks.push_back(task);

            std::getline(file, line);
        }

        file.close();
        std::cout << "Tasks imported from " << filename << " successfully." << std::endl;
    }
    else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}

void FileHandler::readTasksFromFile(const std::string& filename, std::vector<Task>& tasks) {
    std::ifstream file(filename);
    if (file.is_open()) {
        tasks.clear();
        std::string line;
        std::string title, description, dueDate;
        int priority;
        bool recurring;

        while (std::getline(file, title)) {
            std::getline(file, description);
            std::getline(file, dueDate);
            file >> priority;
            file.ignore(); 
            file >> std::boolalpha >> recurring;
            file.ignore(); 

            Task task(title, description, dueDate, priority, recurring);
            tasks.push_back(task);

            std::getline(file, line);
        }

        file.close();
    }
    else {
        std::cout << "Creating new task file: " << filename << std::endl;
    }
}

void FileHandler::writeTasksToFile(const std::string& filename, const std::vector<Task>& tasks) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& task : tasks) {
            task.saveToFile(file);
        }
        file.close();
    }
    else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}
