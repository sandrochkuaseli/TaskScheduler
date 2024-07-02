#include "TaskScheduler.h"
#include "Reminder.h"
#include <iostream>
#include <string>

void printInstructions() {
    std::cout << "Welcome to Task Scheduler!" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "- add: Add a new task" << std::endl;
    std::cout << "- edit: Edit an existing task" << std::endl;
    std::cout << "- remove: Remove a task" << std::endl;
    std::cout << "- list: List all tasks" << std::endl;
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

            std::cout << "Enter task title: ";
            std::getline(std::cin, title);
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            std::cout << "Enter due date (YYYY-MM-DD HH:MM): ";
            std::getline(std::cin, dueDate);
            std::cout << "Enter priority (1-5): ";
            std::cin >> priority;
            std::cin.ignore(); // Consume newline
            std::cout << "Is this task recurring? (yes/no): ";
            std::string recurringInput;
            std::getline(std::cin, recurringInput);
            recurring = (recurringInput == "yes" || recurringInput == "y");

            Task newTask(title, description, dueDate, priority, recurring);
            taskScheduler.addTask(newTask);

        }
        else if (command == "edit") {
            // Implement edit task functionality
            int index;
            std::cout << "Enter task index to edit: ";
            std::cin >> index;
            std::cin.ignore();

            std::string title, description, dueDate;
            int priority;
            bool recurring;

            std::cout << "Enter task title: ";
            std::getline(std::cin, title);
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            std::cout << "Enter due date (YYYY-MM-DD): ";
            std::getline(std::cin, dueDate);
            std::cout << "Enter priority (1-5): ";
            std::cin >> priority;
            std::cin.ignore();
            std::cout << "Is this task recurring? (yes/no): ";
            std::string recurringInput;
            std::getline(std::cin, recurringInput);
            recurring = (recurringInput == "yes" || recurringInput == "y");

            Task updatedTask(title, description, dueDate, priority, recurring);
            taskScheduler.editTask(index - 1, updatedTask);

        }
        else if (command == "remove") {
            // Implement remove task functionality
            int index;
            std::cout << "Enter task index to remove: ";
            std::cin >> index;
            std::cin.ignore(); // Consume newline

            taskScheduler.removeTask(index - 1);

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
        else {
            std::cout << "Invalid command. Type 'help' for instructions." << std::endl;
        }
    }

    return 0;
}
