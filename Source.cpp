#include "TaskScheduler.h"
#include "Reminder.h"
#include <iostream>
#include <string>
#include <sstream>
#include <regex>

/**
  * Print instructions on what the commands do
  */
void printInstructions() {
    std::cout << "Welcome to Task Scheduler!" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "- add: Add a new task" << std::endl;
    std::cout << "- edit: Edit an existing task" << std::endl;
    std::cout << "- remove: Remove a task" << std::endl;
    std::cout << "- remove all: Remove all tasks" << std::endl;
    std::cout << "- list: List all tasks" << std::endl;
    std::cout << "- listP: List all tasks based on priority" << std::endl;
    std::cout << "- show: Show a task with all its attributes" << std::endl;
    std::cout << "- save: Save to local primary 'tasks.txt' file" << std::endl;
    std::cout << "- completed: Set task state to incomplete (All dependants' states are also set to complete)" << std::endl;
    std::cout << "- incomplete: Set task state to incomplete (All dependants' states are also set to incomplete)" << std::endl;
    std::cout << "- export: Export tasks to a file" << std::endl;
    std::cout << "- import: Import tasks from a file (deletes previous tasks)" << std::endl;
    std::cout << "- quit: Quit the program" << std::endl;
}

/**
  * Checks if the given string only contains one single integer and nothing else
  */
bool isSingleInteger(const std::string& input) {
    std::regex pattern(R"(^\s*\d+\s*$)");
    if (!std::regex_match(input, pattern)) {
        
        std::cout << "Invalid input! Please provide single integer!" << std::endl;
        return false;
    }
    return true;
}

/**
  * Checks validity of the provided index
  */
bool checkIndexValidity(int& index, TaskScheduler taskScheduler) {
    std::string intLine;
    std::getline(std::cin, intLine);
    std::istringstream indexStream(intLine);
    if (!isSingleInteger(intLine)) {
        return false;
    }
    indexStream >> index;

    if (index >= taskScheduler.getTasks().size() || index < 0) {
        std::cout << "Invalid index! Out of bounds!" << std::endl;
        return false;
    }

    return true;
}


/**
  * Main function.
  * Manages the flow of the whole program.
  * Reads the given command and performs actions accordingly.
  * Reminder system is also ran in the main function on a seperate thread, it terminates as we quit the application.
  */
int main() {
    TaskScheduler taskScheduler("tasks.txt");
    Reminder reminderSystem(taskScheduler);

    printInstructions();

    reminderSystem.start();

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "add") {
            // Implement add task functionality
            std::string title, description, dueDate, reccOption;
            int priority;
            bool recurring;
            std::vector<int> dependencies;


            while (true) {
                std::cout << "Enter task title: ";
                std::getline(std::cin, title);
                if (!title.empty()) {
                    break;
                }
                std::cout << "Title cannot be empty!" << std::endl;
            }
            
            while (true) {
                std::cout << "Enter task description: ";
                std::getline(std::cin, description);
                if (!description.empty()) {
                    break;
                }
                std::cout << "Description cannot be empty!" << std::endl;
            }

            while (true) {
                std::cout << "Enter due date (YYYY-MM-DD HH:MM): ";
                std::getline(std::cin, dueDate);
                if (dueDate.empty()) {
                    std::cout << "Due date cannot be empty!";
                }
                if (taskScheduler.dueDateValidity(dueDate)) {
                    break;
                }
                std::cout << "Due date format is incorrect!" << std::endl;
            }

            while (true) {

                std::cout << "Enter priority (1-5): ";
                std::string priorityStr;
                std::getline(std::cin, priorityStr);
                std::istringstream priorityStream(priorityStr);

                if (!isSingleInteger(priorityStr)) {
                    continue;
                }

                if (priorityStream >> priority) {
                    if (priority >= 1 && priority <= 5) {
                        break;
                    }
                }
                std::cout << "Priority must be a number between 1 and 5!" << std::endl;
            }

            while (true) {

                std::cout << "Is this task recurring? (yes/no): ";
                std::string recurringInput;
                std::getline(std::cin, recurringInput);
                if (recurringInput == "yes" || recurringInput == "y") {
                    recurring = true;
                    std::cout << "Recurrence options (daily, monthly, yearly): ";
                    while (true) {

                        std::getline(std::cin, reccOption);
                        if (reccOption.empty()) {
                            std::cout << "Field can't be empty!" << std::endl;
                            continue;
                        }
                        if (reccOption == "daily" || reccOption == "monthly" || reccOption == "yearly") {
                            break;
                        }
                        else {
                            std::cout << "Invalid input! Please provide either 'daily', 'monthly', or 'yearly' !" << std::endl;
                        }
                    }
                    break;
                }
                else if (recurringInput == "no" || recurringInput == "n") {
                    recurring = false;
                    break;
                }

                std::cout << "Invalid input for recurrence!" << std::endl;
                
            }

            if (taskScheduler.getTasks().size() > 0) {
                std::cout << "Is this task dependant on other tasks? (yes/no): ";
                std::string dependencyCont;
                std::getline(std::cin, dependencyCont);

                if (dependencyCont == "yes" || dependencyCont == "y") {
                    std::cout << "Write down ID's seperated by spaces: ";
                    std::string dependencyInput;
                    std::getline(std::cin, dependencyInput);
                    std::istringstream dependenciesStream(dependencyInput);
                    int dependecyID;
                    bool valid = true;

                    while (dependenciesStream >> dependecyID) {
                        if (dependecyID < 0 || dependecyID >= taskScheduler.getTasks().size()) {
                            std::cout << "Invalid dependency ID: " << dependecyID << " !" << std::endl;
                            valid = false;
                            break;
                        }
                        dependencies.push_back(dependecyID);
                    }
                    if (!valid) {
                        continue;
                    }
                }
            }
            else {
                std::cout << "First task can't be dependant as no other tasks are in the list!" << std::endl;
            }
            
            int id = taskScheduler.getTasks().size();
            Task newTask(title, description, dueDate, priority, recurring, reccOption, dependencies, id);
            taskScheduler.addTask(newTask);

        }
        else if (command == "edit") {
            // Implement edit task functionality
            int index{};
            std::cout << "Enter task index to edit: ";
            if (!checkIndexValidity(index, taskScheduler)) {
                continue;
            }
            
            std::cout << "Choose one attribute to edit (Title, Description, Due date, Priority, Recurrence, Dependence): ";
            std::string attribute;
            std::getline(std::cin, attribute);

            std::string newAttributeDefinition;
            if (attribute == "Title" || attribute == "title") {
                std::cout << "Previous Title: " << taskScheduler.getTasks()[index].getTitle() << std::endl;
                std::cout << "Provide new Title: ";
                std::getline(std::cin, newAttributeDefinition);
            } 
            else if (attribute == "Description" || attribute == "description") {
                std::cout << "Previous Description: " << taskScheduler.getTasks()[index].getDescription() << std::endl;
                std::cout << "Provide new Description: ";
                std::getline(std::cin, newAttributeDefinition);
            }
            else if (attribute == "Due Date" || attribute == "due date" || attribute == "Due date") {
                std::cout << "Previous Due Date: " << taskScheduler.getTasks()[index].getDueDate() << std::endl;
                std::cout << "Provide new Due Date (YYYY-MM-DD HH:MM): ";
                std::getline(std::cin, newAttributeDefinition);
            }
            else if (attribute == "Priority" || attribute == "priority") {
                std::cout << "Previous Priority: " << taskScheduler.getTasks()[index].getPriority() << std::endl;
                std::cout << "Provide new Priority (1-5): ";
                std::getline(std::cin, newAttributeDefinition);
            }
            else if (attribute == "Recurrence" || attribute == "recurrence") {

                std::cout << "Previous Recurrence: " << (taskScheduler.getTasks()[index].isRecurring() ? "Recurring" : "Not recurring") << std::endl;
                std::cout << "Provide new Recurrence. Options - daily, monthly, yearly, no. Is this task recurring? : ";
                std::getline(std::cin, newAttributeDefinition);

            }
            else if (attribute == "Dependence" || attribute == "dependence") {
                
                std::cout << "Previous Dependence List: ";
                for (int i : taskScheduler.getTasks()[index].getDependencies()) {
                    std::cout << i << " ";
                }
                if (taskScheduler.getTasks()[index].getDependencies().empty()) {
                    std::cout << "Empty!";
                }
                std::cout << std::endl;
                std::cout << "Provide new Dependence List. Write down ID's seperated by spaces (If not dependant on other tasks, type 'empty'): ";
                std::getline(std::cin, newAttributeDefinition);

            }
            else {
                std::cout << "Edit not possible! Invalid attribute provided!" << std::endl;
                continue;
            }

            if (!newAttributeDefinition.empty()) {
                taskScheduler.editTask(index, attribute, newAttributeDefinition);
            }
            else {
                std::cout << "Field cannot be left empty!" << std::endl;
            }
           

        }
        else if (command == "remove") {
            // Implement remove task functionality
            std::cout << "Enter task index to remove: ";
            int index{};
            if (!checkIndexValidity(index, taskScheduler)) {
                continue;
            }
            taskScheduler.removeTask(index);
            
        }
        else if (command == "list") {
            // Implement list tasks functionality
            taskScheduler.listTasks(taskScheduler.getTasks());

        }
        else if (command == "listP") {
            taskScheduler.listTasksByPriority();
        }
        else if (command == "export") {
            // Implement export tasks functionality
            std::cout << "Enter file name to export to (If file does not exist, it will be created): ";
            std::string filename;
            std::getline(std::cin, filename);
            taskScheduler.exportTasks(filename);

        }
        else if (command == "import") {
            // Implement import tasks functionality
            std::string importFilename;
            std::cout << "Enter filename to import from: ";
            std::getline(std::cin, importFilename);
            taskScheduler.removeAllTasks();
            taskScheduler.importTasks(importFilename);

        }
        else if (command == "quit") {
            std::cout << "Exiting program..." << std::endl;
            break;

        }
        else if (command == "remove all") {
            taskScheduler.removeAllTasks();
        }
        else if (command == "show") {
            std::cout << "Enter task index to show: ";
            int index{};
            if (!checkIndexValidity(index, taskScheduler)) {
                continue;
            }
            taskScheduler.showTask(index);
        }
        else if (command == "save") {
            taskScheduler.exportTasks("tasks.txt");
        }
        else if (command == "completed") {
            std::cout << "Enter task index you completed: ";
            int index{};
            if (!checkIndexValidity(index, taskScheduler)) {
                continue;
            }
            taskScheduler.setComplete(index, true);
            
        }
        else if (command == "incomplete") {
            std::cout << "Enter task index to set to incomplete: ";
            int index{};
            if (!checkIndexValidity(index, taskScheduler)) {
                continue;
            }
            taskScheduler.setComplete(index, false);
        }
        else if (command == "help") {
            printInstructions();
        }
        else {
            std::cout << "Invalid command. Type 'help' for instructions." << std::endl;
        }
    }

    taskScheduler.exportTasks("tasks.txt");
    reminderSystem.stop();

    return 0;
}
