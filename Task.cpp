#include "Task.h"

Task::Task(const std::string& title, const std::string& description, const std::string& dueDate, int priority, bool recurring, std::vector<int>& dependecies, int id)
    : title(title), description(description), dueDate(dueDate), priority(priority), recurring(recurring), dependecies(dependecies), taskID(id) {
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

void Task::setTitle(const std::string& newTitle)
{
    title = newTitle;
}

void Task::setDescription(std::string& newDescription)
{
    description = newDescription;
}

void Task::setDueDate(std::string& newDueDate)
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

void Task::saveToFile(std::ofstream& file) const {
    file << title << '\n';
    file << description << '\n';
    file << dueDate << '\n';
    file << priority << '\n';
    file << recurring << '\n';
    for (int i : dependecies) {

        file << i + 1 << " ";
    }
    file << '\n';
    for (int i : dependants) {

        file << i + 1 << " ";
    }
    file << '\n';
    file << taskID << '\n';
    file << "---\n";
}
