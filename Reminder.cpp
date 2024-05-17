#include "Reminder.h"
#include <iostream>
#include <ctime>
#include <iomanip>


Reminder::Reminder(const TaskScheduler& taskScheduler, std::chrono::minutes interval)
	: taskScheduler_(taskScheduler), checkInterval_(interval), running_(false){}

void Reminder::start()
{
	running_ = true;

	std::thread([this])() {
		while (running_)
		{
			checkForReminder();
			std::this_thread:
		}
	}
}

void Reminder::stop()
{
}

void Reminder::checkForReminder() const
{
}

std::string Reminder::timeToString(time_t time) const
{
	return std::string();
}
