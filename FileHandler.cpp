#include "FileHandler.h"
#include "TaskScheduler.h"
#include <fstream>
#include <iostream>
#include <sstream>

/*
    Exports set of tasks to a provided file.
    If file does not exist new file is created.
*/
void FileHandler::exportTasks(const std::string& filename, const std::vector<Task>& tasks) {
    std::ofstream file(filename, std::ios::trunc);
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

/*
    Imports tasks from a file.
    It is assumed that the format is correct mostly, as this format of tasks can only be exported from this Taskscheduler.
*/
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

            bool recurring;
            std::string recurringStr;
            std::getline(file, recurringStr);
            std::istringstream recurringStream(recurringStr);
            recurringStream >> recurring;

            std::string recurrOption;
            std::getline(file, recurrOption);

            std::vector<int> dependencies;
            std::string dependencyStr;
            std::getline(file, dependencyStr);
            std::istringstream dependenciesStream(dependencyStr);
            int dependencyInt;
            while (dependenciesStream >> dependencyInt) {
                dependencies.push_back(dependencyInt);
            }

            std::vector<int> dependendants;
            std::string dependendantsStr;
            std::getline(file, dependendantsStr);
            std::istringstream dependendantsStream(dependendantsStr);
            int dependendantsInt;
            while (dependendantsStream >> dependendantsInt) {
                dependendants.push_back(dependendantsInt);
            }

            int id;
            std::string idString;
            std::getline(file, idString);
            std::istringstream idStream(idString);
            idStream >> id;

            bool completed;
            std::string completedStr;
            std::getline(file, completedStr);
            std::istringstream completedStream(completedStr);
            completedStream >> completed;

            Task task(title, description, dueDate, priority, recurring, recurrOption, dependencies, id);
            
            task.setTaskID(tasks.size());
            if (TaskScheduler::checkFormat(task)) {
                
                for (int i : dependendants) {

                    task.setDependant(i);
                }

                task.setCompleted(completed);
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
