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

    for (Task task : taskScheduler.getTasks()) {
        if (!task.getCompleted()) {
            std::istringstream dueDateStream(task.getDueDate());
            int year, month, day, hour, minute;
            char dash1, dash2, colon;

            dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

            if (localTime.tm_year + 1900 == year && localTime.tm_mon + 1 == month &&
                localTime.tm_mday == day && localTime.tm_hour == hour && localTime.tm_min == minute) {
                
                std::cout << std::endl;
                std::cout << "Reminder: Task '" << task.getTitle() << "' is due now!" << std::endl;
                std::cout << "> ";

                // To add: Check if complete if not remind every hour until the end of the day and then remove the task or set as completed
            }

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

            if (daysToDate == 0) {
                std::cout << std::endl;
                std::cout << "Reminder: Task '" << task.getTitle() << "' is due soon!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate == 1)
            {
                std::cout << std::endl;
                std::cout << "Reminder: Task '" << task.getTitle() << "' is due in 1 day!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate < 8 && daysToDate > 1) {
                std::cout << std::endl;
                std::cout << "Reminder: Task '" << task.getTitle() << "' is due in " << daysToDate << " days!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate == 30 ) {
                std::cout << std::endl;
                std::cout << "Reminder: Task '" << task.getTitle() << "' is due in 1 month!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate % 30 == 0) {
                std::cout << std::endl;
                std::cout << "Reminder: Task '" << task.getTitle() << "' is due in " << daysToDate / 30 << " months!" << std::endl;
                std::cout << "> ";
            }
        }

    }

}
