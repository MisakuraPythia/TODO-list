#include <fstream>
#include <iostream>
#include <vector>
#include "task.h"
#include <limits>
#include <typeinfo>
#include <sstream>

void saveTasks() {
    std::ofstream file("tasks.txt");

    for (Task& task : tasks) {
        file << task.name << "|" << task.priority << "|" << task.finished << '\n';
    }
    file.close();
}

Task addTaskLoad(Task& create) {
    return create;
}

void loadTasks() {
    std::ifstream file("tasks.txt");

    if (file.is_open()) {
        std::string line{};
        while (getline(file, line)) {
            std::stringstream ss{line};
            Task load{};
            getline(ss, load.name, '|');
            ss >> load.priority;
            ss.ignore(1);
            ss >> load.finished;
            tasks.push_back(addTaskLoad(load));
        }
        file.close();
    }
}

void printPriority(const int priority) {
    for (const Task& task : tasks) {
        if (task.priority == priority) {
            std::cout << task.name << ": ";
            if (task.finished) {
                std::cout << "COMPLETE\n";
            }
            else {
                std::cout << "INCOMPLETE\n";
            }
        }
    }
}

Task addTask() {
    Task create{};
    std::cout << "What is your task\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, create.name);
    std::cout << "And the priority? (choose a number between 1 and 3, with 3 being the lowest\n";
    while (typeid(create.priority) != typeid(int) || (create.priority < 1 || create.priority > 3))
    {
        std::cin >> create.priority;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return create;
}


int main()
{
    loadTasks();
    int choice{};
    while (choice != 3) {
        std::cout << "\n---My TODO list---\n";
        std::cout << "1. View List\n";
        std::cout << "2. Add to List\n";
        std::cout << "3. Quit\n";

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (choice < 1 || choice > 3)
        {
            std::cin >> choice;
        }

        if (choice == 1)
        {
            int listChoice{};
            while (listChoice != 4) {
                std::cout << "\n---My Tasks List---\n";
                std::cout << "\n   -Priority 1 tasks-  \n";
                printPriority(1);
                std::cout << "\n   -Priority 2 tasks-  \n";
                printPriority(2);
                std::cout << "\n   -Priority 3 tasks-  \n";
                printPriority(3);

                std::cout << "\n\n1. Mark a task as complete/incomplete\n2. Remove a task\n3. Remove all completed (not daily)\n4. Exit\n";

                std::cin >> listChoice;
                if (listChoice == 1) {
                    std::string taskName{};
                    std::cout << "\nWhat is the name of the task you would like to mark as complete/incomplete?\n";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, taskName);
                    bool found{false};
                    for (Task& task : tasks) {
                        if (task.name == taskName) {
                            task.finished = !task.finished;
                            std::cout << task.name << " status successfully updated!\n";
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        std::cout <<"No task with the name of '" << taskName << "'\n";
                    }
                }
                if (listChoice == 2) {
                    std::string taskName{};
                    std::cout << "\nWhat is the name of the task you would like to remove?\n";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, taskName);
                    bool found{false};
                    int taskDeletion{};
                    for (int i = 0; i < tasks.size(); i++) {
                        if (tasks[i].name == taskName) {
                            found = true;
                            taskDeletion = i;
                            break;
                        }
                    }
                    if (found) {
                        tasks.erase(tasks.begin () + taskDeletion);
                        std::cout << taskName << " successfully removed!\n";
                    }
                    if (!found) {
                        std::cout <<"No task with the name of '" << taskName << "'\n";
                    }

                }

                if (listChoice == 3) {
                    char remove{};
                    std::cout << "Are you sure you would like to remove all finished tasks? (y/n)";
                    while (remove != 'y' && remove != 'n') {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin >> remove;
                        if (remove == 'y') {
                            std::vector<int> finishedTasks;
                            for (int i = 0; i < tasks.size(); i++) {
                                if (tasks[i].finished == true && tasks[i].priority != 1) {
                                    finishedTasks.push_back(i);
                                }
                            }
                            for (int i = 0; i < finishedTasks.size(); i++) {
                                tasks.erase(tasks.begin () + finishedTasks[i] - i);
                            }
                            break;
                        }
                        if (remove == 'n') {
                            break;
                        }
                    }

                }
                if (listChoice == 4) {
                    break;
                }
            }
        }

        if (choice == 2) {
            bool want{true};
            while (want) {
                tasks.push_back(addTask());
                std::cout << "Task has been created!\n";
                saveTasks();
                std::cout << "Would you like to create another? (y/n)\n";

                while (true){
                    char selection{};
                    std::cin >> selection;

                    if (selection == 'y') {
                        break;
                    }
                    if (selection == 'n') {
                        want = false;
                        break;
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }
    }
    saveTasks();
    return 0;
}


