//
// Created by Misakura on 15/03/2026.
//

#ifndef TODO_TASK_H
#define TODO_TASK_H
#include <string>


struct Task {
    std::string name{};
    int priority{};
    bool finished{false};
};

inline  std::vector<Task> tasks{};

#endif //TODO_TASK_H