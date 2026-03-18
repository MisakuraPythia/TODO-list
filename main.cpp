#include <iostream>
#include <vector>
#include "task.h"
#include <limits>

void printPriority(int priority) {
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].priority == priority) {
            std::cout << tasks[i].name << ": ";
            if (tasks[i].finished) {
                std::cout << "COMPLETE\n";
            }
            else {
                std::cout << "INCOMPLETE\n";
            }
        }
    }
}


void printTasks() {
    std::cout << "\n---My Tasks List---\n";
    std::cout << "   -Priority 1 tasks-  \n";
    printPriority(1);
    std::cout << "   -Priority 2 tasks-  \n";
    printPriority(2);
    std::cout << "   -Priority 3 tasks-  \n";
    printPriority(3);
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

        while (choice < 1 || choice > 3)
        {
            std::cin >> choice;
        }

        if (choice == 1)
        {
            printTasks();
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
    choice = 0;
    }


    return 0;
}


