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
    // Getters
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDueDate() const;
    int getPriority() const;
    bool isRecurring() const;
    
    // Setters
    void setTitle(const std::string& newTitle);
    void setDescription(std::string& newDescription);
    void setDueDate(std::string& newDueDate);
    void setPriority(int newPriority);
    void setRecurring(bool newRecurring);

    void saveToFile(std::ofstream& file) const;
};

#endif // TASK_H
