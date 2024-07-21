/** /file Task.cpp
  * Task class source file.
  * Contains function definitions.
  */

#include "Task.h"

Task::Task(const std::string& title, const std::string& description, const std::string& dueDate, int priority, bool recurring, std::string& recurringOption, std::vector<int>& dependecies, int id)
    : title(title), description(description), dueDate(dueDate), priority(priority), recurring(recurring), dependecies(dependecies), taskID(id), recurringOption(recurringOption) {
    completed = false;
}

std::string Task::getTitle() const {
    return title;
}

std::string Task::getDescription() const {
    return description;
}

std::string Task::getDueDate() const {
    return dueDate;
}

int Task::getPriority() const {
    return priority;
}

bool Task::isRecurring() const {
    return recurring;
}

std::string Task::getRecurringOpt() const
{
    return (recurringOption.empty() ? "None" : recurringOption);
}

std::vector<int> Task::getDependencies() const
{
    return dependecies;
}

std::vector<int> Task::getDependants() const
{
    return dependants;
}

int Task::getTaskID() const
{
    return taskID;
}

bool Task::getCompleted() const
{
    return completed;
}

void Task::setTitle(const std::string& newTitle)
{
    title = newTitle;
}

void Task::setDescription(const std::string& newDescription)
{
    description = newDescription;
}

void Task::setDueDate(const std::string& newDueDate)
{
    dueDate = newDueDate;
}

void Task::setPriority(int newPriority)
{
    priority = newPriority;
}

void Task::setRecurring(bool newRecurring)
{
    recurring = newRecurring;
}

void Task::setRecurringOpt(std::string recurringOpt)
{
    recurringOption = recurringOpt;
}

void Task::setDependency(int taskId)
{
    dependecies.push_back(taskId);
}

void Task::setTaskID(int taskId)
{
    taskID = taskId;
}

void Task::setDependant(int taskId)
{
    dependants.push_back(taskId);
}

void Task::setCompleted(bool complete)
{
    completed = complete;
}

/**
  * Remove id from dependency list
  */
void Task::removeDependency(int taskId)
{
    for (int j = 0; j < dependecies.size(); j++) {
        if (dependecies[j] == taskId) {
            dependecies.erase(dependecies.begin() + j);
            break;
        }
    }
}

/**
  * Remove id from dependant list
  */
void Task::removeDependant(int taskId) {
    for (int j = 0; j < dependants.size(); j++) {
        if (dependants[j] == taskId) {
            dependants.erase(dependants.begin() + j);
            break;
        }
    }
}

/**
  * Adjust the dependency list
  * Used when some tasks change ID's and we need to adjust the dependency list accordingly
  */
void Task::adjustDependencyList(int taskId)
{
    for (int i = 0; i < dependecies.size(); i++) {
        if (dependecies[i] >= taskId) {
            dependecies[i] -= 1;
        }
    }
}

/**
  * Adjust the dependant list
  * Used when some tasks change ID's and we need to adjust the dependant list accordingly
  */
void Task::adjustDependantList(int taskId)
{
    for (int i = 0; i < dependants.size(); i++) {
        if (dependants[i] >= taskId) {
            dependants[i] -= 1;
        }
    }
}

/**
  * Write a single task to file
  */
void Task::saveToFile(std::ofstream& file) const {
    file << title << '\n';
    file << description << '\n';
    file << dueDate << '\n';
    file << priority << '\n';
    file << recurring << '\n';
    file << recurringOption << '\n';
    for (int i : dependecies) {

        file << i << " ";
    }
    file << '\n';
    for (int i : dependants) {

        file << i << " ";
    }
    file << '\n';
    file << taskID << '\n';
    file << completed << '\n';
    file << "---\n";
}
