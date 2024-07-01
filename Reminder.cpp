#include "Reminder.h"
#include <iostream>
#include <ctime>
#include <iomanip>


Reminder::Reminder(TaskScheduler& taskScheduler) : taskScheduler(taskScheduler), running(false) {}

Reminder::~Reminder() {
    stop();
}

void Reminder::start() {
    if (!running) {
        running = true;
        reminderThread = std::thread([this]() {
            while (running) {
                checkReminders();
                std::this_thread::sleep_for(std::chrono::minutes(1));
            }
            });
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

void Reminder::checkReminders() {
    std::cout << "Checking reminders..." << std::endl;

}