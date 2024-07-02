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
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);

    for (const auto& task : taskScheduler.getTasks()) {
        std::istringstream dueDateStream(task.getDueDate());
        int year, month, day, hour, minute;
        char dash1, dash2, colon;

        dueDateStream >> year >> dash1 >> month >> dash2 >> day >> hour >> colon >> minute;

        if (localTime->tm_year == year - 1900 && localTime->tm_mon + 1 = month &&
            localTime->tm_mday == day && localTime->tm_hour == hour &&
            localTime->tm_min == minute) {
            std::cout << "Reminder: Task '" << task.getTitle() << "' is due now!";
        }
    }

}