#ifndef REMINDER_H
#define REMINDER_H

#include "TaskScheduler.h"
#include <chrono>
#include <thread>

class Reminder {
private:
    TaskScheduler& taskScheduler;
    bool running;
    std::thread reminderThread;

public:
    ~Reminder();
    Reminder(TaskScheduler& taskScheduler);

    void start();
    void stop();
    void run();
    void checkTasksAndRemind();
};


#endif // !REMINDER_H
