#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sstream> // For string manipulation

// Task structure
struct Task {
    int id;
    std::string title;
    std::string description;
    int priority; // 1 (high) to 3 (low)
    time_t due_date;
    bool completed; 
};

// Task list singleton
class TaskList {
private:
    std::vector<Task> tasks;
    std::mutex taskMutex;
    std::condition_variable taskCV;

    // Private constructor for singleton
    TaskList() {}

public:
    static TaskList& getInstance() {
        static TaskList instance;
        return instance;
    }

    void addTask(const Task& task) {
        std::lock_guard<std::mutex> lock(taskMutex);
        tasks.push_back(task);
        taskCV.notify_one();
    }

    std::vector<Task> getTasks() {
        std::lock_guard<std::mutex> lock(taskMutex);
        return tasks;
    }

    void removeTask(int id) {
        std::lock_guard<std::mutex> lock(taskMutex);
        tasks.erase(std::remove_if(tasks.begin(), tasks.end(), 
            [&](const Task& t) { return t.id == id; }), tasks.end());
        taskCV.notify_one();
    }

    void updateTask(int id, const std::string& field, const std::string& newValue) {
        std::lock_guard<std::mutex> lock(taskMutex);
        for (auto& task : tasks) {
            if (task.id == id) {
                if (field == "title") {
                    task.title = newValue;
                } else if (field == "description") {
                    task.description = newValue;
                } else if (field == "priority") {
                    task.priority = std::stoi(newValue);
                } else if (field == "due_date") {
                    // Handle parsing of due_date (not shown)
                } else if (field == "completed") {
                    task.completed = (newValue == "true");
                }
                taskCV.notify_one();
                return;
            }
        }
    }
};

// Shared memory segment for task list
int shm_id;
TaskList* sharedTaskList;

// Function to handle signal (SIGINT - Ctrl+C)
void signalHandler(int signum) {
    std::cout << "Exiting task manager...\n";
    // Detach shared memory
    shmdt(sharedTaskList);
    // Remove shared memory segment
    shmctl(shm_id, IPC_RMID, NULL);
    exit(0);
}

// Background thread to check for overdue tasks
void backgroundThread() {
    while (true) {
        std::vector<Task> tasks = TaskList::getInstance().getTasks();
        for (const Task& task : tasks) {
            if (!task.completed && time(NULL) > task.due_date) {
                std::cout << "Task '" << task.title << "' is overdue!\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(10)); // Check every 10 seconds
    }
}

// Function to parse a date string (YYYY-MM-DD) into a time_t object
time_t parseDate(const std::string& dateString) {
    struct tm tm = {};
    std::istringstream ss(dateString);
    char delimiter;
    ss >> tm.tm_year >> delimiter >> tm.tm_mon >> delimiter >> tm.tm_mday;
    tm.tm_year -= 1900; // Adjust year for tm_year
    tm.tm_mon -= 1;    // Adjust month for tm_mon
    return mktime(&tm);
}

int main() {
    // Set up signal handler for Ctrl+C
    signal(SIGINT, signalHandler);

    // Create shared memory segment
    shm_id = shmget(IPC_PRIVATE, sizeof(TaskList), 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment
    sharedTaskList = (TaskList*)shmat(shm_id, NULL, 0);
    if (sharedTaskList == (TaskList*)-1) {
        perror("shmat");
        exit(1);
    }

    // Start background thread
    std::thread background(backgroundThread);

    std::cout << "Welcome to the Task Manager!\n";

    int nextTaskId = 1; // For assigning unique IDs to tasks

    while (true) {
        std::string command;
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        // Parse command and execute corresponding action
        if (command == "add") {
            std::cout << "Enter task title: ";
            std::string title;
            std::getline(std::cin, title);

            std::cout << "Enter task description: ";
            std::string description;
            std::getline(std::cin, description);

            std::cout << "Enter task priority (1-3, 1 being highest): ";
            int priority;
            std::cin >> priority;
            std::cin.ignore(); // Consume newline character

            std::cout << "Enter due date (YYYY-MM-DD): ";
            std::string dueDateString;
            std::getline(std::cin, dueDateString);
            time_t dueDate = parseDate(dueDateString);

            Task newTask = {nextTaskId++, title, description, priority, dueDate, false}; 
            sharedTaskList->addTask(newTask);
            std::cout << "Task added!\n";

        } else if (command == "view") {
            std::vector<Task> tasks = sharedTaskList->getTasks();
            if (tasks.empty()) {
                std::cout << "No tasks yet.\n";
            } else {
                std::cout << "Tasks:\n";
                for (const Task& task : tasks) {
                    std::cout << "ID: " << task.id << "\n";
                    std::cout << "Title: " << task.title << "\n";
                    std::cout << "Description: " << task.description << "\n";
                    std::cout << "Priority: " << task.priority << "\n";
                    std::cout << "Due Date: " << ctime(&task.due_date); // Use ctime for formatting
                    std::cout << "Completed: " << (task.completed ? "Yes" : "No") << "\n";
                    std::cout << "---------------------\n";
                }
            }

        } else if (command == "remove") {
            std::cout << "Enter task ID to remove: ";
            int id;
            std::cin >> id;
            std::cin.ignore(); // Consume newline character

            sharedTaskList->removeTask(id);
            std::cout << "Task removed!\n";

        } else if (command == "update") {
            std::cout << "Enter task ID to update: ";
            int id;
            std::cin >> id;
            std::cin.ignore(); // Consume newline character

            std::cout << "Enter field to update (title, description, priority, due_date, completed): ";
            std::string field;
            std::getline(std::cin, field);

            std::cout << "Enter new value: ";
            std::string newValue;
            std::getline(std::cin, newValue);

            sharedTaskList->updateTask(id, field, newValue);
            std::cout << "Task updated!\n";

        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Invalid command.\n";
        }
    }

    // Join background thread
    background.join();

    // Detach and remove shared memory
    shmdt(sharedTaskList);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}