#include <fstream>
#include <iostream>
#include <vector>
#include "task.h"
#include <limits>

void saveTasks() {
    std::ofstream file("tasks.txt");

    for (Task& task : tasks) {
        file << task.name << "|" << task.priority << "|" << task.finished << '\n';
    }
    file.close();
}


void printPriority(const int priority, std::vector<Task> &taskByPriority) {
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
    while (create.priority < 1 || create.priority > 3)
    {
        std::cin >> create.priority;
    }
    return create;
}


int main()
{

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

            std::cout << "\n---My Tasks List---\n";
            std::cout << "\n   -Priority 1 tasks-  \n";
            std::vector<Task> priority1{};
            printPriority(1, priority1);
            std::cout << "\n   -Priority 2 tasks-  \n";
            std::vector<Task> priority2{};
            printPriority(2, priority2);
            std::cout << "\n   -Priority 3 tasks-  \n";
            std::vector<Task> priority3{};
            printPriority(3, priority3);
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
            std::cin.get(); // wait for Enter
        }

        if (choice == 2) {
            bool want{true};
            while (want) {
                tasks.push_back(addTask());
                std::cout << "Task has been created!\n";
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


