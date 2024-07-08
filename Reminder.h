#ifndef REMINDER_H
#define REMINDER_H

#include "TaskScheduler.h"
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>

/**
  * Reminder handles the task of notifying the user about upcoming tasks.
  * Runs in the background on two threads.
  */
class Reminder {
private:
    TaskScheduler& taskScheduler;
    std::atomic<bool> running;
    std::thread reminderThreadMin;
    std::thread reminderThreadHour;
    std::mutex mtx;
    std::condition_variable cv;

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
