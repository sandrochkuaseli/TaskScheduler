#ifndef TASK_H
#define TASK_H

#include <string>
#include <fstream>
#include <vector>

class Task {
private:
    std::string title;
    std::string description;
    std::string dueDate;
    int priority;
    bool recurring;
    std::vector<int> dependecies;
    std::vector<int> dependants;
    int taskID;
    bool completed;

public:
    Task(const std::string& title, const std::string& description, const std::string& dueDate, int priority, bool recurring, std::vector<int>& dependecies, int id);
    // Getters
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDueDate() const;
    int getPriority() const;
    bool isRecurring() const;
    std::vector<int> getDependencies() const;
    std::vector<int> getDependants() const;
    int getTaskID() const;

    
    // Setters
    void setTitle(const std::string& newTitle);
    void setDescription(std::string& newDescription);
    void setDueDate(std::string& newDueDate);
    void setPriority(int newPriority);
    void setRecurring(bool newRecurring);
    void setDependency(int taskId);
    void setTaskID(int taskId);
    void setDependant(int taskId);

    void saveToFile(std::ofstream& file) const;
};

#endif // TASK_H
