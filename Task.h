#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime>


class Task {

private:
	std::string title;
	std::string description;
	time_t dueDate;
	int priority;
	bool recurring;

public:
	Task();
	Task(std::string title_, std::string description_, time_t dueDate_, int priority_, bool recurring_);

	// GETTERS
	std::string getTitle() const;
	std::string getDescription() const;
	time_t getDueDate() const;
	int getPriority() const;
	bool isRecurring() const;


	// SETTERS
	void setTitle(std::string title);
	void setDescription(std::string description);
	void setDueDate(time_t dueDate);
	void setPriority(int priority);
	void setRecurring(bool recurring);

	// Serialize task to a string (for saving to file)
	std::string serialize() const;

	// Deserialize task from a string (for reading from file)
	static Task deserialize(const std::string& serializedTask);


};



#endif