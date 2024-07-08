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

    auto currentDate = std::chrono::system_clock::from_time_t(std::mktime(&localTime));

    std::tm futureTime = {};
    std::istringstream dueDateStream(dueDateStr);
    dueDateStream >> std::get_time(&futureTime, "%Y-%m-%d %H:%M");

    futureTime.tm_hour = 0;
    futureTime.tm_min = 0;
    futureTime.tm_sec = 0;

    auto futureDate = std::chrono::system_clock::from_time_t(std::mktime(&futureTime));

    auto diff = (std::chrono::duration_cast<std::chrono::hours>(futureDate - currentDate).count());
    int days = static_cast<int>(diff);

    return days;
}

int getDaysInMonth2(int year, int month) {
    // Checks for leap year
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return 29;
        }
        else {
            return 28;
        }
    }

    switch (month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        break;
    }

    return 31;
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

            
            if (task.getRecurringOpt() == "daily") {
                dueDateTimeT += 3600 * 24;
            }
            else if(task.getRecurringOpt() == "monthly"){
                dueDateTimeT += 3600 * 24 * getDaysInMonth2(year, month);
            }
            else if (task.getRecurringOpt() == "yearly") {
                dueDateTimeT += 3600 * 24 * 365;
            }

            std::cout << "\nReminder: Task '" << task.getTitle() << "' is due now! It has already been completed but it is a " << task.getRecurringOpt() << " recurring task!" << std::endl;
            std::cout << "Setting task to incomplete! Respective, dependants and dependecies are also set to incomplete!" << std::endl;
            
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
            task.setCompleted(false);

            std::cout << "New Due Date Time: " << newdueStr << std::endl;

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

            //std::cout << "Hours to date: " << daysToDate << std::endl;
            if (daysToDate < -1) {
                std::cout << "\nReminder: Task '" << task.getTitle() << "' was due on " << task.getDueDate() << " and is still incomplete!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate < 24 ){
                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due soon!" << std::endl;
                std::cout << "> ";
            }
            else if (daysToDate > 24 && daysToDate < 48)
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
