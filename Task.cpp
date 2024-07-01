#include "Task.h"

Task::Task(const std::string& title, const std::string& description, const std::string& dueDate, int priority, bool recurring)
    : title(title), description(description), dueDate(dueDate), priority(priority), recurring(recurring) {}

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

void Task::saveToFile(std::ofstream& file) const {
    file << title << '\n';
    file << description << '\n';
    file << dueDate << '\n';
    file << priority << '\n';
    file << recurring << '\n';
    file << "---\n";
}
