#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "Task.h"
#include <vector>

class FileHandler {
public:
    static void exportTasks(const std::string& filename, const std::vector<Task>& tasks);
    static void importTasks(const std::string& filename, std::vector<Task>& tasks);

};

#endif // FILE_HANDLER_H
