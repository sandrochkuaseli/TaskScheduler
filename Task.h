#ifndef TASK_H
#define TASK_H

#include <string>
#include <fstream>

class Task {
private:
    std::string title;
    std::string description;
    std::string dueDate;
    int priority;
    bool recurring;

public:
    Task(const std::string& title, const std::string& description, const std::string& dueDate, int priority, bool recurring);

    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDueDate() const;
    int getPriority() const;
    bool isRecurring() const;

    void saveToFile(std::ofstream& file) const;
};

#endif // TASK_H
