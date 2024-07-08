#include "TaskScheduler.h"
#include "FileHandler.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <chrono>
#include <ctime>
#include <queue>

/**
  * Constructor
  * Imports tasks from local 'tasks.txt' file
  */
TaskScheduler::TaskScheduler(const std::string& filename) : filename(filename) {
    loadTasksFromFile();
}

/**
  * Handles addition of tasks from command line
  */
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


/**
  * Edits one of the attributes
  */
void TaskScheduler::editTask(int index, std::string attribute, std::string newAttributeDefinition) {

    if (attribute == "Title" || attribute == "title") {
        tasks[index].setTitle(newAttributeDefinition);
    }
    else if (attribute == "Description" || attribute == "description") {
        tasks[index].setDescription(newAttributeDefinition);
    }
    else if (attribute == "Due Date" || attribute == "due date" || attribute == "Due date") {
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

        if (newAttributeDefinition == "daily" || newAttributeDefinition == "monthly" || newAttributeDefinition == "yearly") {
            tasks[index].setRecurring(true);
            tasks[index].setRecurringOpt(newAttributeDefinition);
        }
        else if (newAttributeDefinition == "no" || newAttributeDefinition == "n") {
            tasks[index].setRecurring(false);
        }
        else {
            std::cout << "Invalid input! Choose one of four options provided!" << std::endl;
        }

    }
    else if (attribute == "Dependence" || attribute == "dependence") {
        for (int i : tasks[index].getDependencies()) {
            tasks[i].removeDependant(index);
            tasks[index].removeDependency(i);
        }
        std::vector<int> newDependencies;
        int val;
        std::istringstream dependencyStream(newAttributeDefinition);
        while (dependencyStream >> val) {
            newDependencies.push_back(val);
        }

        for (int i : newDependencies) {
            if(i == index){
                std::cout << "Can't add '" << i << "', as it is this task's ID! It is already dependants on itself!" << std::endl;
            }
            else if (0 < i && i >= tasks.size()) {
                std::cout << "No such task with ID '" << i << "' exists!" << std::endl;

            }
            else {
                tasks[index].setDependency(i);
                tasks[i].setDependant(index);
            }
        }

    }
} 


/**
  * Removes a task from the task set.
  * Adjusts index and dependants and dependencies in relation to other tasks accordingly.
  */
void TaskScheduler::removeTask(int index) {
    if (index >= 0 && index < tasks.size()) {

        for (int depIndex : tasks[index].getDependencies()) {

            tasks[depIndex].removeDependant(index);

        }

        for (int depIndex : tasks[index].getDependants()) {

            tasks[depIndex].removeDependency(index);

        }

        tasks.erase(tasks.begin() + index);

        for (int j = index; j < tasks.size(); j++) {
            tasks[j].adjustDependencyList(index);
            tasks[j].adjustDependantList(index);
            tasks[j].setTaskID(j);
        }
        std::cout << "Task sucessfully removed!" << std::endl;

    }
    else {
        std::cout << "Invalid task index." << std::endl;
    }
}


/**
  *  Lists all tasks
  */
void TaskScheduler::listTasks(std::vector<Task> taskVec) const {
    if (tasks.empty()) {
        std::cout << "No tasks found!" << std::endl;
    }
    else {
        for (size_t i = 0; i < taskVec.size(); ++i) {
            std::cout <<"ID: " << taskVec[i].getTaskID() << " - Title: " << taskVec[i].getTitle() << " - Due: " << taskVec[i].getDueDate()
                << " - Priority: " << taskVec[i].getPriority() << " - " << (taskVec[i].getCompleted() ? "COMPLETED!" : "INCOMPLETE!" ) << std::endl;
        }
    }
}


/**
  * Lists all tasks based on the priority (highest priority first).
  */
void TaskScheduler::listTasksByPriority() const
{
    std::vector<Task> sortedByPriority = tasks;
    std::sort(sortedByPriority.begin(), sortedByPriority.end(), [](const Task& a, const Task& b) {
        return a.getPriority() > b.getPriority();
        });

    listTasks(sortedByPriority);

}

/**
  *  Shows every attribute of the task
  */
void TaskScheduler::showTask(int index) {
    if (index >= 0 && index < tasks.size()) {
        std::cout << "Title: " <<  tasks[index].getTitle() << std::endl;
        std::cout << "Description: " << tasks[index].getDescription() << std::endl;
        std::cout << "Due date: " << tasks[index].getDueDate() << std::endl;
        std::cout << "Priority: " << tasks[index].getPriority() << std::endl;

        
        std::cout << "Recurring: " <<  (tasks[index].isRecurring() ? "Yes, " + tasks[index].getRecurringOpt() : "NO") << std::endl;

        
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

        std::cout << "Completed: " << (tasks[index].getCompleted() ? "COMPLETED!" : "INCOMPLETE!") << std::endl;

    }
}

void TaskScheduler::exportTasks(std::string filename) const {
    FileHandler::exportTasks(filename, tasks);
}

/**
  * Import tasks at any point
  */
void TaskScheduler::importTasks(const std::string& importFilename) {
    FileHandler::importTasks(importFilename, tasks);
}

/**
  * Initial import of the tasks
  */
void TaskScheduler::loadTasksFromFile() {
    FileHandler::importTasks(filename, tasks);
}

/**
  * Removes all tasks from the task set
  */
void TaskScheduler::removeAllTasks() {
    tasks.clear();
}

/**
  * Checks validity of provided due date.
  * Checks if the provided format is valid and then checks whether the due date is later than current date.
  */
bool TaskScheduler::dueDateValidity(const std::string& dueDateString) {
    std::regex pattern("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}");

    if (std::regex_match(dueDateString, pattern)) {
        int year, month, day, hour, minute;
        char dash1, dash2, colon;
        
        std::istringstream dueDateStream(dueDateString);

        dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::tm localTime;
#if defined(_MSC_VER) // For Microsoft Visual Studio
        localtime_s(&localTime, &currentTime);
#else // For other compilers
        localtime_r(&currentTime, &localTime);
#endif

        if (12 < month || getDaysInMonth(year, month) < day || 24 < hour || 59 < minute) {
            
            return false;
        }


        if (localTime.tm_year + 1900 > year) {
            return false;
        }
        else if (localTime.tm_year + 1900 == year) {
            if (localTime.tm_mon > month ) {
                return false;
            }
            else if (localTime.tm_mon == month) {
                if (localTime.tm_mday > day) {
                    return false;
                }
                else if (localTime.tm_mday == day) {
                    if (localTime.tm_hour > hour ) {
                        return false;
                    }
                    else if (localTime.tm_hour == hour) {
                        if (localTime.tm_min >= minute ) {
                            return false;
                        }
                    }
                }
            }
        }
        
        return true;

    }

    return false;
}

/**
  * Function to check overall format of the task 
  */
bool TaskScheduler::checkFormat(const Task& task)
{
    bool valid = true;
    // Title not more than 200 char
    if (task.getTitle().length() > 200) {
        std::cout << "!! Task not added to the scheduler: Title exceeds the character limit !!" << std::endl;
        valid = false;
    }
    else if (!dueDateValidity(task.getDueDate())) {
        std::cout << "!! Task not added to the scheduler: Due date format is incorrect !!" << std::endl;
        valid = false;
    }
    if (task.isRecurring() && task.getRecurringOpt() != "daily" && task.getRecurringOpt() != "monthly" && task.getRecurringOpt() != "yearly") {
        std::cout << "!!Task not added to the scheduler : Invalid recurrence option provided !!" << std::endl;
    }
    else if (1 < task.getPriority() && task.getPriority() > 5) {
        std::cout << "!! Task not added to the scheduler: Priority out of bounds !!" << std::endl;
        valid = false;
    }

    return valid;
}

/**
  * Setter for the dependants list, push one element
  */
void TaskScheduler::setDependants(int taskId, int dependantId)
{
    tasks[taskId].getDependants().push_back(dependantId);
}

/**
  * Getter method. Returns the set of tasks
  */
const std::vector<Task>& TaskScheduler::getTasks() const {
    return tasks;
}

/**
  * Getter method. Returns the set of tasks
  */
std::vector<Task>& TaskScheduler::getTasks() { 
    return tasks;
}

/**
  * Sets completion status of task.
  * Updates other tasks accordingly.
  * We don't do anything if the task is already completed and we set it to completed again.
  * If tasks is set to true, all the tasks dependant to it are also automatically set to true and dependencies are also updated accordingly.
  * If all dependants of a task are complete than the task it self is also complete.
  * Same goes for when we set task to incomplete.
  */
void TaskScheduler::setComplete(int taskId, bool complete)
{
    if (tasks[taskId].getCompleted() != complete) {

        tasks[taskId].setCompleted(complete);

        std::queue<Task> taskQueueDependency;
        taskQueueDependency.push(tasks[taskId]);
        std::vector<bool> visitedDependency(tasks.size(), false);
        visitedDependency[taskId] = true;

        while (!taskQueueDependency.empty()) {
            Task task = taskQueueDependency.front();
            taskQueueDependency.pop();

            for (int i : task.getDependencies()) {
                if (!visitedDependency[i]) {

                    visitedDependency[i] = true;

                    if (complete) {
                        bool isComplete = true;
                        for (int j : tasks[i].getDependants()) {
                            if (!tasks[j].getCompleted()) {
                                isComplete = false;
                                break;
                            }
                        }

                        if (isComplete) {
                            tasks[i].setCompleted(true);
                            taskQueueDependency.push(tasks[i]);
                        }
                    }
                    else {
                        if (tasks[i].getCompleted() != complete) {
                            tasks[i].setCompleted(complete);
                            taskQueueDependency.push(tasks[i]);
                        }
                    }
                
                }
            }
        }


        std::queue<Task> taskQueueDependant;
        taskQueueDependant.push(tasks[taskId]);
        std::vector<bool> visitedDependants(tasks.size(), false);
        visitedDependants[taskId] = true;

        while (!taskQueueDependant.empty()) {
            Task task = taskQueueDependant.front();
            taskQueueDependant.pop();

            for (int i : task.getDependants()) {
                if (!visitedDependants[i]) {
                    visitedDependants[i] = true;
                    if (tasks[i].getCompleted() != complete) {
                        tasks[i].setCompleted(complete);
                        taskQueueDependant.push(tasks[i]);
                    }
                }
            
            }
        }
    }
    
}


Task& TaskScheduler::operator[](size_t index) {
    return tasks[index];
}

const Task& TaskScheduler::operator[](size_t index) const {
    return tasks[index];
}
