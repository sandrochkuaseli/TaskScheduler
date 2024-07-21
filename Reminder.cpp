/** /file Reminder.cpp
  * Reminder class source file.
  * Contains all the function definitions.
  */


#include "Reminder.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>


/**
  * Reminder class constructor
  */
Reminder::Reminder(TaskScheduler& taskScheduler) :
    taskScheduler(taskScheduler), running{ false } {}


Reminder::~Reminder() {
    stop();
}

/**
  * Start two threads, one checks every 59 seconds other check checks every 59 minutes
  */
void Reminder::start() {
    if (!running.load()) {
        running.store(true);

        reminderThreadMin = std::thread(&Reminder::runEveryMin, this);
        reminderThreadHour = std::thread(&Reminder::runEveryHour, this);
    }
}



/**
  * Terminate threads
  * We use cv and mtx to tell tasks to wake up for termination
  */
void Reminder::stop() {
    if (running.load()) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            running.store(false);

        }

        cv.notify_all();

        if (reminderThreadMin.joinable()) {
            reminderThreadMin.join();

        }
        if (reminderThreadHour.joinable()) {
            reminderThreadHour.join();
        }

    }
}

/**
  * Runs checksTasksAndRemindNow() every 59 seconds
  */

void Reminder::runEveryMin() {
    std::unique_lock<std::mutex> lock(mtx);
    while (running.load()) {
        checkTasksAndRemindNow();
        if (cv.wait_for(lock, std::chrono::seconds(59), [this] { return !running.load(); })) {
            break;
        }
    }
}

/**
  * Runs checksTasksAndRemindLonger() every 59 minutes
  */
void Reminder::runEveryHour()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (running.load()) {
        checkTasksAndRemindLonger();
        if (cv.wait_for(lock, std::chrono::minutes(59), [this] { return !running.load(); })) {
            break;
        }
    }
}


/**
  * Returns int which represents difference between current date and due date in hours
  * 
  * @param[in] dueDateStr dueDateStr is passed to the function which is in valid format YYYY-MM-DD HH:MM and then it is parsed to calculate time in hours to current time
  * @return Returns hours left to current date from given date
  */
int countHoursToDate(std::string dueDateStr) {
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

/**
  * Reminds about tasks that are due at the moment
  * Renews due date if the due date is now and the task is still incomplete
  * Renews due date according to recurring settings
  */
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
                dueDateTimeT += 3600 * 24 * TaskScheduler::getDaysInMonth(year, month);
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

/**
  * Reminds about tasks that are either past their due date or are due in the future (months, weeks, days)
  */
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
            int hoursToDate = countHoursToDate(task.getDueDate());
            int daysToDate = hoursToDate / 24;
            std::istringstream dueDateStream(task.getDueDate());

            int year, month, day, hour, minute;
            char dash1, dash2, colon;

            dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

            if (hoursToDate < -1) {
                std::cout << "\nReminder: Task '" << task.getTitle() << "' was due on " << task.getDueDate() << " and is still incomplete!" << std::endl;
                std::cout << "> ";
            }
            else if (hoursToDate > 0 && hoursToDate < 24 ){
                std::cout << "\nReminder: Task '" << task.getTitle() << "' is due soon!" << std::endl;
                std::cout << "> ";
            }
            else if (hoursToDate > 24 && hoursToDate < 36)
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
