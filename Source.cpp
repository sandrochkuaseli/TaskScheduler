#include "TaskScheduler.h"
#include "Reminder.h"
#include <iostream>
#include <string>
#include <sstream>

void printInstructions() {
    std::cout << "Welcome to Task Scheduler!" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "- add: Add a new task" << std::endl;
    std::cout << "- edit: Edit an existing task" << std::endl;
    std::cout << "- remove: Remove a task" << std::endl;
    std::cout << "- remove all: Remove all tasks" << std::endl;
    std::cout << "- list: List all tasks" << std::endl;
    std::cout << "- show: Show a task with all its attributes" << std::endl;
    std::cout << "- save: Save to local primary 'tasks.txt' file" << std::endl;
    std::cout << "- completed: Set task to completed" << std::endl;
    std::cout << "- export: Export tasks to a file" << std::endl;
    std::cout << "- import: Import tasks from a file" << std::endl;
    std::cout << "- quit: Quit the program" << std::endl;
}

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
            std::string title, description, dueDate;
            int priority;
            bool recurring;
            std::vector<int> dependencies;

            std::cout << "Enter task title: ";
            std::getline(std::cin, title);
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            std::cout << "Enter due date (YYYY-MM-DD HH:MM): ";
            std::getline(std::cin, dueDate);
            
            std::cout << "Enter priority (1-5): ";
            std::string priorityStr;
            std::getline(std::cin, priorityStr);
            std::istringstream priorityStream(priorityStr);
            priorityStream >> priority;

            std::cout << "Is this task recurring? (yes/no): ";
            std::string recurringInput;
            std::getline(std::cin, recurringInput);
            if (recurringInput == "yes" || recurringInput == "y") {
                recurring = true;
            }
            else if (recurringInput == "no" || recurringInput == "n") {
                recurring = false;
            }
            else {
                std::cout << "Invalid input for recurrence!" << std::endl;
                continue;
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
                    while (dependenciesStream >> dependecyID) {
                        dependencies.push_back(dependecyID);
                    }
                }
            }
            else {
                std::cout << "First task can't be dependant as no other tasks are in the list!" << std::endl;
            }
            
            int id = taskScheduler.getTasks().size();
            Task newTask(title, description, dueDate, priority, recurring, dependencies, id);
            taskScheduler.addTask(newTask);

        }
        else if (command == "edit") {
            // Implement edit task functionality
            int index;
            std::cout << "Enter task index to edit: ";
            std::cin >> index;
            std::cin.ignore();
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
            else if (attribute == "Due Date" || attribute == "due date") {
                std::cout << "Previous Due Date: " << taskScheduler.getTasks()[index].getDueDate() << std::endl;
                std::cout << "Provide new Due Date: ";
                std::getline(std::cin, newAttributeDefinition);
            }
            else if (attribute == "Priority" || attribute == "priority") {
                std::cout << "Previous Priority: " << taskScheduler.getTasks()[index].getPriority() << std::endl;
                std::cout << "Provide new Priority (1-5): ";
                std::getline(std::cin, newAttributeDefinition);
            }
            else if (attribute == "Recurrence" || attribute == "recurrence") {
                std::string recurring = "Not recurring";
                if (taskScheduler.getTasks()[index].isRecurring()) {
                    recurring = "Recurring";
                }
                std::cout << "Previous Recurrence: " << recurring << std::endl;
                std::cout << "Provide new Recurrence. Is this task recurring? (yes/no): ";
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
                std::cout << "Provide new Depenedce List. Write down ID's seperated by spaces (If not dependant on other tasks, type 'empty'): ";
                std::getline(std::cin, newAttributeDefinition);

            }
            else {
                std::cout << "Edit not possible! Invalid task index or invalid attribute provided!" << std::endl;
            }

            if (!newAttributeDefinition.empty()) {
                taskScheduler.editTask(index, attribute, newAttributeDefinition);
            }
           

        }
        else if (command == "remove") {
            // Implement remove task functionality
            int index;
            std::cout << "Enter task index to remove: ";
            std::cin >> index;
            std::cin.ignore();

            taskScheduler.removeTask(index);

        }
        else if (command == "list") {
            // Implement list tasks functionality
            taskScheduler.listTasks();

        }
        else if (command == "export") {
            // Implement export tasks functionality
            taskScheduler.exportTasks();

        }
        else if (command == "import") {
            // Implement import tasks functionality
            std::string importFilename;
            std::cout << "Enter filename to import from: ";
            std::getline(std::cin, importFilename);

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
            int index;
            std::cout << "Enter task index to show: ";
            std::cin >> index;
            std::cin.ignore();
            taskScheduler.showTask(index);
        }
        else if (command == "save") {
            taskScheduler.saveTasksToFile();
        }
        else if (command == "completed") {
            std::cout << "Enter task index you completed: ";
            int index;
            std::cin >> index;
            std::cin.ignore();
            taskScheduler.setComplete(index, true);
        }
        else {
            std::cout << "Invalid command. Type 'help' for instructions." << std::endl;
        }
    }

    taskScheduler.saveTasksToFile();
    reminderSystem.stop();

    return 0;
}
