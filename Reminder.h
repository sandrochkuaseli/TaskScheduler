#ifndef REMINDER_H
#define REMINDER_H

#include "TaskScheduler.h"
#include <chrono>
#include <thread>

class Reminder {
private:
	const TaskScheduler& taskScheduler_;
	bool running_;
	std::chrono::minutes checkInterval_;

public:
	Reminder(const TaskScheduler& taskScheduler, std::chrono::minutes interval = std::chrono::minutes(1));

	void start();

	void stop();

	void checkForReminder() const;

	std::string timeToString(time_t time) const;

};


#endif // !REMINDER_H
