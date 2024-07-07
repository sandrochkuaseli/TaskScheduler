#include "Reminder.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>


Reminder::Reminder(TaskScheduler& taskScheduler) :
    taskScheduler(taskScheduler), running(false) {}

Reminder::~Reminder() {
    stop();
}

void Reminder::start() {
    if (!running) {
        running = true;

        reminderThreadMin = std::thread(&Reminder::runEveryMin, this);
        reminderThreadHour = std::thread(&Reminder::runEveryHour, this);
    }
}

void Reminder::stop() {
    if (running) {
        running = false;
        if (reminderThreadMin.joinable()) {
            reminderThreadMin.join();
        }
        if (reminderThreadHour.joinable()) {
            reminderThreadHour.join();
        }
    }
}

void Reminder::runEveryMin() {
    while (running) {
        checkTasksAndRemindNow();
        std::this_thread::sleep_for(std::chrono::seconds(59));
    }
}

void Reminder::runEveryHour()
{
    while (running) {
        checkTasksAndRemindLonger();
        std::this_thread::sleep_for(std::chrono::minutes(59));
    }
}


int countDaysToDate(std::string dueDateStr) {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
#if defined(_MSC_VER) // For Microsoft Visual Studio
    localtime_s(&localTime, &currentTime);
#else // For other compilers
    localtime_r(&currentTime, &localTime);
#endif

    localTime.tm_hour = 0;
    localTime.tm_min = 0;
    localTime.tm_sec = 0;
    std::time_t currentDate = std::mktime(&localTime);

    std::tm futureTime = {};
    std::istringstream dueDateStream(dueDateStr);
    dueDateStream >> std::get_time(&futureTime, "%Y-%m-%d %H:%M");

    futureTime.tm_hour = 0;
    futureTime.tm_min = 0;
    futureTime.tm_sec = 0;

    std::time_t futureDate = std::mktime(&futureTime);

    auto diff = std::difftime(futureDate, currentDate);
    int days = static_cast<int>(diff / (60 * 60 * 24));

    return days;
}

void Reminder::checkTasksAndRemindNow() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
#if defined(_MSC_VER) // For Microsoft Visual Studio
    localtime_s(&localTime, &currentTime);
#else // For other compilers
    localtime_r(&currentTime, &localTime);
#endif

    for (Task& task : taskScheduler.getTasks()) {

        std::istringstream dueDateStream(task.getDueDate());

        localTime.tm_sec = 0;
        std::time_t currentTimeT = std::mktime(&localTime);

        std::tm dueDateTime = {};
        int year, month, day, hour, minute;
        char dash1, dash2, colon;

        dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

        dueDateTime.tm_year = year - 1900;
        dueDateTime.tm_mon = month - 1;   
        dueDateTime.tm_mday = day;
        dueDateTime.tm_hour = hour - 1;
        dueDateTime.tm_min = minute;
        dueDateTime.tm_sec = 0;

        std::time_t dueDateTimeT = std::mktime(&dueDateTime);

        if(currentTimeT == dueDateTimeT && !task.getCompleted()){
                
            std::cout << "\nReminder: Task '" << task.getTitle() << "' is due now!" << std::endl;
            std::cout << "Reminder: Task '" << task.getTitle() << "' has not yet been completed! Reminding again in 1 hour! (Due date will be adjusted) " << std::endl;
                
            dueDateTimeT += 3600;
            std::tm newDueDateTime;
#if defined(_MSC_VER) // For Microsoft Visual Studio
            localtime_s(&newDueDateTime, &dueDateTimeT);
#else // For other compilers
            localtime_r(&dueDateTimeT, &newDueDateTime);
#endif
            std::ostringstream oss;
            oss << std::put_time(&newDueDateTime, "%Y-%m-%d %H:%M");
            std::string newdueStr = oss.str();
            task.setDueDate(newdueStr);

            std::cout << "New Due Date Time: " << newdueStr << std::endl;

            std::cout << "> ";

        }else if (currentTimeT == dueDateTimeT && task.getCompleted() && task.isRecurring()) {

            std::cout << "\nReminder: Task '" << task.getTitle() << "' is due now! It has already been completed but it is a recurring task!" << std::endl;

            while (true) {
                std::string dueDate;
                std::cout << "Please enter new due date (YYYY-MM-DD HH:MM): ";
                std::getline(std::cin, dueDate);
                if (dueDate.empty()) {
                    std::cout << "Due date cannot be empty!";
                }
                if (taskScheduler.dueDateValidity(dueDate)) {
                    task.setDueDate(dueDate);
                    break;
                }
                std::cout << "Due date format is incorrect!" << std::endl;
            }

            std::cout << "> ";
        }

   
        
    }

}

void Reminder::checkTasksAndRemindLonger() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
#if defined(_MSC_VER) // For Microsoft Visual Studio
    localtime_s(&localTime, &currentTime);
#else // For other compilers
    localtime_r(&currentTime, &localTime);
#endif

    for (Task task : taskScheduler.getTasks()) {
        if (!task.getCompleted()) {
            int daysToDate = countDaysToDate(task.getDueDate());

            std::istringstream dueDateStream(task.getDueDate());

            int year, month, day, hour, minute;
            char dash1, dash2, colon;

            dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

            std::cout << "\nDays to date:" << daysToDate << std::endl;
            if (daysToDate == 0 && localTime.tm_hour > hour) {
                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due soon!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate == 1)
            {

                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due tomorrow!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate < 8 && daysToDate > 1) {

                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due in " << daysToDate << " days!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate == 30 ) {

                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due in 1 month!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate % 30 == 0 && daysToDate != 0) {

                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due in " << daysToDate / 30 << " months!" << std::endl;
                std::cout << "> ";
            }
        }

    }

}
