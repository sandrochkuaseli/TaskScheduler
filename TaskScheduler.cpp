#include "TaskScheduler.h"
#include "FileHandler.h"
#include <iostream>
#include <regex>
#include <sstream>

TaskScheduler::TaskScheduler(const std::string& filename) : filename(filename) {
    loadTasksFromFile();
}

void TaskScheduler::addTask(const Task& task) {
    bool correctFormat = true;
    
    if (checkFormat(task)) {
        tasks.push_back(task);
        
        if (!task.getDependencies().empty()) {
            for (int i : task.getDependencies()) {
                if (0 < i && i > tasks.size()) {
                    std::cout << "No such task with ID '" << i << "' exists!" << std::endl;
                    correctFormat = false;
                    break;
                }
                else {

                    tasks[i].setDependant(task.getTaskID());
                }
            }

        }
        
        if (!correctFormat) {
            tasks.pop_back();
            std::cout << "!! Task not added to the scheduler: Dependancy list includes task that does not exist !!" << std::endl;
        }
    }


}

void TaskScheduler::editTask(int index, std::string attribute, std::string newAttributeDefinition) {
    if (index >= 0 && index < tasks.size()) {
        if (attribute == "Title" || attribute == "title") {
            tasks[index].setTitle(newAttributeDefinition);
        }
        else if (attribute == "Description" || attribute == "description") {
            tasks[index].setDescription(newAttributeDefinition);
        }
        else if (attribute == "Due Date" || attribute == "due date") {
            if (dueDateValidity(newAttributeDefinition)) {

                tasks[index].setDueDate(newAttributeDefinition);
            }
            else {
                std::cout << "Invalid input! Date format is incorrect!" << std::endl;
            }
        }
        else if (attribute == "Priority" || attribute == "priority") {
            std::istringstream priorityStream(newAttributeDefinition);
            int priority;
            priorityStream >> priority;
            if (priority > 0 && priority <= 5) {

                tasks[index].setPriority(priority);
            }
            else {
                std::cout << "Invalid input!" << std::endl;
            }
        }
        else if (attribute == "Recurrence" || attribute == "recurrence") {

            if (newAttributeDefinition == "yes" || newAttributeDefinition == "y") {
                tasks[index].setRecurring(true);
            }
            else if (newAttributeDefinition == "no" || newAttributeDefinition == "n") {
                tasks[index].setRecurring(false);
            }
            else {
                std::cout << "Invalid input!" << std::endl;
            }

        }
    } 
    else {
        std::cout << "Task index out of bounds!" << std::endl;
    }
}

void TaskScheduler::removeTask(int index) {
    if (index >= 0 && index < tasks.size()) {

        for (int depIndex : tasks[index].getDependencies()) {

            tasks[depIndex].removeDependant(index);

            for (int i : tasks[index].getDependencies()) {
                std::cout << i << ", ";
            }
            std::cout << std::endl;

        }

        for (int depIndex : tasks[index].getDependants()) {

            tasks[depIndex].removeDependency(index);

            for (int i : tasks[depIndex].getDependencies()) {
                std::cout << i << ", ";
            }
            std::cout << std::endl;

        }

        tasks.erase(tasks.begin() + index);

        for (int j = index; j < tasks.size(); j++) {
            tasks[j].adjustDependencyList(index);
            tasks[j].adjustDependantList(index);
            tasks[j].setTaskID(j);
        }

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
            std::cout << i << ". " << tasks[i].getTitle() << " - Due: " << tasks[i].getDueDate()
                << " - Priority: " << tasks[i].getPriority() << std::endl;
        }
    }
}

void TaskScheduler::showTask(int index) {
    if (index >= 0 && index < tasks.size()) {
        std::cout << "Title: " <<  tasks[index].getTitle() << std::endl;
        std::cout << "Description: " << tasks[index].getDescription() << std::endl;
        std::cout << "Due date: " << tasks[index].getDueDate() << std::endl;
        std::cout << "Priority: " << tasks[index].getPriority() << std::endl;
        std::cout << "Recurring: " << tasks[index].isRecurring() << std::endl;
        if (!tasks[index].getDependencies().empty()) {
            std::cout << "Dependecy list: ";
            for (int i : tasks[index].getDependencies()) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

        }
        else {
            std::cout << "No dependencies!" << std::endl;
        }

        if (!tasks[index].getDependants().empty()) {
            std::cout << "Dependant list: ";
            for (int i : tasks[index].getDependants()) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

        }
        else {
            std::cout << "No dependants!" << std::endl;
        }
            
        std::cout << "Task ID: " << tasks[index].getTaskID() << std::endl;

    }
}

void TaskScheduler::exportTasks() const {
    FileHandler::exportTasks(filename, tasks);
}

void TaskScheduler::importTasks(const std::string& importFilename) {
    FileHandler::importTasks(importFilename, tasks, idCount);
}

void TaskScheduler::loadTasksFromFile() {
    FileHandler::importTasks(filename, tasks, idCount);
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

Task& TaskScheduler::operator[](size_t index) {
    return tasks[index];
}

const Task& TaskScheduler::operator[](size_t index) const {
    return tasks[index];
}

int TaskScheduler::getIdCount()
{
    return idCount;
}
