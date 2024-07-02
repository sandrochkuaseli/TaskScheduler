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
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}

void Reminder::checkTasksAndRemind() {
    std::cout << "Checking reminders..." << std::endl;

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime;
    std::tm localTime;
#if defined(_MSC_VER) // Microsoft Visual Studio
    localtime_s(&localTime, &currentTime);
#else // Other compilers that support POSIX
    localtime_r(&currentTime, &localTime);
#endif
    for (const auto& task : taskScheduler.getTasks()) {
        std::istringstream dueDateStream(task.getDueDate());
        int year, month, day, hour, minute;
        char dash1, dash2, colon;

        dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;


        auto dueDate = std::chrono::system_clock::from_time_t(std::mktime(&localTime));
        dueDate += std::chrono::hours(hour) + std::chrono::minutes(minute);


        if (now >= dueDate &&
            now < dueDate + std::chrono::minutes(1)) {
            std::cout << "Reminder: Task '" << task.getTitle() << "' is due now!";
        }
    }

}