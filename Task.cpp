#include "Task.h"

#include <sstream>

Task::Task(){}

Task::Task(std::string title_, std::string description_, time_t dueDate_, int priority_, bool recurring_) : 
	title(title_), description(description_), dueDate(dueDate_), priority(priority_), recurring(recurring_){}


// GETTERS

std::string Task::getTitle() const{ return title; }

std::string Task::getDescription() const { return description; }

time_t Task::getDueDate() const { return dueDate; }

int Task::getPriority() const { return priority; }

bool Task::isRecurring() const { return recurring; }

// SETTERS

void Task::setTitle(std::string title_) { title = title_; }
void Task::setDescription(std::string description_) { description = description_;  }
void Task::setDueDate(time_t dueDate_) { dueDate = dueDate_;  }
void Task::setPriority(int priority_) { priority = priority_; }
void Task::setRecurring(bool recurring_) { recurring = recurring_; }


// SERIALIZE AND DESERIALIZE FUNCTIONS FOR READING AND SAVING TASKS FROM FILES
std::string Task::serialize() const {
    // Format: title|description|dueDate|priority|recurring
    return title + "|" + description + "|" + std::to_string(dueDate) + "|" + std::to_string(priority) + "|" + (recurring ? "1" : "0");
}

static Task deserialize(const std::string& serializedTask) {
    Task task;
    std::stringstream ss(serializedTask);
    std::string token;
    int i = 0;
    while (std::getline(ss, token, '|')) {
        switch (i) {
        case 0: task.setTitle(token); break;
        case 1: task.setDescription(token); break;
        case 2: task.setDueDate(std::stol(token)); break;
        case 3: task.setPriority(std::stoi(token)); break;
        case 4: task.setRecurring(token == "1"); break;
        }
        i++;
    }
    return task;
}
