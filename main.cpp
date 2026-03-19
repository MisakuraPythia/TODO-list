#include <fstream>
#include <iostream>
#include <vector>
#include "task.h"
#include <limits>
#include <typeinfo>
#include <sstream>
#include <iomanip>
#include <chrono>

int getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&now_c);

    int year = localTime.tm_year + 1900;   // tm_year is years since 1900
    int month = localTime.tm_mon + 1;      // tm_mon is 0–11
    int day = localTime.tm_mday;           // day of the month 1–31

    return year * 10000 + month * 100 + day;  // YYYYMMDD
}


// function checks to see if day has passed, then updates the daily tasks to incomplete
void newDay() {
    std::ifstream file(std::string(getenv("HOME")) + "/.todo_day.txt");
    std::string oldTime{};
    int currentDate{getCurrentDate()};
    bool isNewDay = false;
    if (file.is_open()) {
        std::getline(file, oldTime);
        int oldTimeNum = std::stoi(oldTime);
        if (oldTimeNum != currentDate) {
            isNewDay = true;
        }
    }
    file.close();

    if (isNewDay) {
        std::ofstream outputToFile(std::string(getenv("HOME")) + "/.todo_day.txt");
        if (outputToFile.is_open()) {
            outputToFile << currentDate;
            for (Task& task : tasks) {
                if (task.priority == 1) {
                    task.finished = false;
                }
            }
        }
        outputToFile.close();
    }

}

void saveTasks() {
    std::ofstream file(std::string(getenv("HOME")) + "/.todo_tasks.txt");

    for (Task& task : tasks) {
        file << task.name << "|" << task.priority << "|" << task.finished << '\n';
    }
    file.close();
}

Task addTaskLoad(Task& create) {
    return create;
}

void loadTasks() {
    std::ifstream file(std::string(getenv("HOME")) + "/.todo_tasks.txt");

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

void printPriority(const int& priority, const int& length) {
    for (const Task& task : tasks) {
        if (task.priority == priority) {
            std::cout << std::left << std::setw(length + 2) << task.name << ": ";
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
    std::cout << "And the priority? (1. Daily, 2. Asap, 3. Long-term)\n";
    while (typeid(create.priority) != typeid(int) || (create.priority < 1 || create.priority > 3))
    {
        std::cin >> create.priority;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return create;
}

int getLongestTask() {
    int length{};
    for (Task& task : tasks) {
        if (task.name.size() > length) {
            length = static_cast<int>(task.name.size());
        }
    }
    return length;
}

int main(int argc, char* argv[]){
    if (argc > 1) {
        std::string command = argv[1];

        if (argc > 4) {
            std::cout << "Too many commands";
            return 0;
        }
        if (command == "add") {
            if (argc < 4) {
                std::cout << "add command requires <priority> and <task_name>";
            }
            if (argc == 4) {
                int argcPriority = std::stoi(argv[2]);
                std::string taskName = argv[3];
                if ((argcPriority >= 1 && argcPriority <= 3) && argc == 4 ) {
                    loadTasks();
                    newDay();
                    Task create{};
                    create.name = taskName;
                    create.priority = argcPriority;
                    tasks.push_back(create);
                    saveTasks();
                    return 0;
                }
            }
            std::cout << "invalid commands";
            return 0;
        }
        else if (command == "remove") {
            if (argc == 3) {
                std::string taskName{argv[2]};
                loadTasks();
                newDay();
                int deletion{};
                bool match{false};
                for (int i = 0; i < tasks.size(); i++) {
                    if (tasks[i].name == taskName) {
                        deletion = i;
                        match = true;
                        break;

                    }
                }
                if (match) {
                    tasks.erase(tasks.begin() + deletion);
                    saveTasks();
                    return 0;
                }
                {
                    std::cout << "No task with this name\n";
                    return 0;
                }
            }
            std::cout << "invalid";
            return 0;
        }
        else if (command == "complete") {
            if (argc == 3) {
                std::string taskName{argv[2]};
                loadTasks();
                newDay();
                bool match{false};
                for (Task& task: tasks) {
                    if (task.name == taskName) {
                        match = true;
                        task.finished = true;
                        break;
                    }
                }
                if (match){
                    saveTasks();
                }
                else {
                    std::cout << "No task with this name\n";
                }
                return 0;
            }
            std::cout << "invalid commands";
            return 0;
        }

        else if (command == "incomplete") {
            if (argc == 3) {
                std::string taskName{argv[2]};
                loadTasks();
                newDay();
                bool match{false};
                for (Task& task: tasks) {
                    if (task.name == taskName) {
                        match = true;
                        task.finished = false;
                        break;
                    }
                }
                if (match){
                    saveTasks();
                    return 0;
                }
                std::cout << "No task with this name\n";
                return 0;
            }
            std::cout << "invalid commands";
            return 0;
        }

        else if (command == "list") {
            loadTasks();
            newDay();
            int length = getLongestTask();
            std::string priorityOneTitle{"-Daily tasks-"};
            std::string priorityTwoTitle{"-Asap tasks-"};
            std::string priorityThreeTitle{"-Long term tasks-"};
            std::cout << "\n---My Tasks List---\n";
            std::cout << '\n' << priorityOneTitle << '\n';
            printPriority(1, length);
            std::cout << '\n' << priorityTwoTitle << '\n';
            printPriority(2, length);
            std::cout << '\n' << priorityThreeTitle << '\n';
            printPriority(3, length);
            return 0;
        }
    }



    loadTasks();
    newDay();
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
            int length = getLongestTask();
            std::string priorityOneTitle{"-Daily tasks-"};
            std::string priorityTwoTitle{"-Asap tasks-"};
            std::string priorityThreeTitle{"-Long term tasks-"};
            while (listChoice != 4) {
                std::cout << "\n---My Tasks List---\n";
                std::cout << '\n' << priorityOneTitle << '\n';
                printPriority(1, length);
                std::cout << '\n' << priorityTwoTitle << '\n';
                printPriority(2, length);
                std::cout << '\n' << priorityThreeTitle << '\n';
                printPriority(3, length);

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


