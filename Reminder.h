#ifndef REMINDER_H
#define REMINDER_H

#include "TaskScheduler.h"
#include <chrono>
#include <thread>

class Reminder {
private:
    TaskScheduler& taskScheduler;
    bool running;
    std::thread reminderThreadMin;
    std::thread reminderThreadHour;

public:
    ~Reminder();
    Reminder(TaskScheduler& taskScheduler);

    void start();
    void stop();
    void runEveryMin();
    void runEveryHour();
    void checkTasksAndRemindNow();
    void checkTasksAndRemindLonger();
};


#endif // !REMINDER_H
