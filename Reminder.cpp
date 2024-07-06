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

        reminderThread = std::thread(&Reminder::run, this);
    }
}

void Reminder::stop() {
    if (running) {
        running = false;
        if (reminderThread.joinable()) {
            reminderThread.join();
        }
    }
}

void Reminder::run() {
    while (running) {
        checkTasksAndRemind();
        std::this_thread::sleep_for(std::chrono::seconds(59));
    }
}

void Reminder::checkTasksAndRemind() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
#if defined(_MSC_VER) // For Microsoft Visual Studio
    localtime_s(&localTime, &currentTime);
#else // For other compilers
    localtime_r(&currentTime, &localTime);
#endif

    for (Task task : taskScheduler.getTasks()) {
        std::istringstream dueDateStream(task.getDueDate());
        int year, month, day, hour, minute;
        char dash1, dash2, colon;
        
        dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

        if (localTime.tm_year + 1900 == year && localTime.tm_mon + 1 == month &&
            localTime.tm_mday == day && localTime.tm_hour == hour && localTime.tm_min == minute) {
            std::cout << "Reminder: Task '" << task.getTitle() << "' is due now!" << std::endl;

            // To add: Check if complete if not remind every hour until the end of the day and then remove the task or set as completed
        }
        else if ((localTime.tm_year + 1900 == year && localTime.tm_mon + 1 == month - 1 && localTime.tm_mday == day && month != 1) || (month == 1 && localTime.tm_year + 1900 == year - 1 && localTime.tm_mon + 1 == 12 && localTime.tm_mday == day)) {
            std::cout << "Reminder: Task '" << task.getTitle() << "' is due in 1 month!" << std::endl;
        }
    }

}