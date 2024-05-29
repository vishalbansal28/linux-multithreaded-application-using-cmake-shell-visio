# linux-multithreaded-application-using-cmake-shell-visio

## Project: Simple Multithreaded Task Manager 

This is a command-line task manager application built using C++ that demonstrates:

* **Multithreading:** Creating and managing multiple threads for concurrent execution (a UI thread and a background thread).
* **Inter-Process Communication (IPC):** Sharing data between threads using shared memory.
* **CMake Build System:** Automating the build process with CMake.
* **Shell Scripting:** Using a shell script to easily build and run the application.
* **Basic Design Patterns:**  Implementing singleton and observer patterns (partially implemented; could be expanded).
* **Design Visualization:**  The project structure is illustrated using a Microsoft Visio diagram (not included in the repository but recommended).

## Features

* **Add Tasks:** Add new tasks with titles, descriptions, priorities, and due dates.
* **View Tasks:** Display a list of all added tasks.
* **Remove Tasks:** Delete tasks by their unique IDs.
* **Update Tasks:**  Modify task details (title, description, priority, due date, completed status).
* **Overdue Task Notifications:** A background thread periodically checks for overdue tasks and prints notifications to the console.

## Technologies Used

* **C++11:** The programming language used for implementing the application logic.
* **POSIX Threads (pthreads):** For creating and managing threads.
* **Shared Memory (POSIX Shared Memory):**  For inter-thread communication, allowing the background thread to access and update the task list. 
* **CMake:** For managing the build process, including finding dependencies, setting compiler flags, and generating build files.
* **Bash Scripting:** A simple Bash script is used to automate the building and running of the application.
* **Microsoft Visio (Optional):**  Recommended for creating visual diagrams of the project's architecture, class relationships, and data flow.

## Project Structure

```
task-manager/
├── src/
│   └── main.cpp          # Main C++ source code file
├── CMakeLists.txt        # CMake build configuration file
├── scripts/
│   └── run.sh            # Shell script to build and run 
└── diagrams/             # (Optional) Directory to store Visio diagrams
    └── task_manager_architecture.vsdx 
```

## Getting Started

1. **Prerequisites:** 
   * Linux operating system (tested on Ubuntu)
   * GCC compiler (or compatible) 
   * CMake build system
   * Make build tool

2. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/linux-multithreaded-application-using-cmake-shell-visio.git
   cd linux-multithreaded-application-using-cmake-shell-visio 
   ```

3. **Build the Project:**

   ```bash
   ./scripts/run.sh
   ```

4. **Run the Application:**

   ```bash
   ./bin/task-manager 
   ```

## Usage

1. **Add a task:**
   ```
   Enter command: add
   Enter task title: Finish Project Report
   Enter task description: Complete the final sections and proofread
   Enter task priority (1-3, 1 being highest): 1
   Enter due date (YYYY-MM-DD): 2024-03-10
   Task added!
   ```

2. **View all tasks:**
   ```
   Enter command: view
   ```

3. **Remove a task:**
   ```
   Enter command: remove
   Enter task ID to remove: 1
   Task removed!
   ```

4. **Update a task:**
   ```
   Enter command: update
   Enter task ID to update: 2
   Enter field to update (title, description, priority, due_date, completed): priority
   Enter new value: 2
   Task updated!
   ```

5. **Exit the task manager:**
   ```
   Enter command: exit
   ```

## Design Considerations

* **Singleton Pattern:** The `TaskList` class is implemented as a singleton to ensure there's only one instance of the task list, making it globally accessible.

* **Observer Pattern (Potential Enhancement):** The project could be extended to use the Observer pattern. This would allow the UI thread to be automatically notified when the background thread detects an overdue task or when changes are made to the task list. 

## Contributing

Feel free to fork the repository and submit pull requests to contribute to the project. Here are some potential improvements:

* **Enhanced Date Handling:** Implement more robust date parsing and validation using a dedicated date/time library.
* **User Interface:** Develop a graphical user interface using a GUI toolkit like Qt or GTK for a more user-friendly experience.
* **Data Persistence:**  Store tasks in a file to allow tasks to persist between sessions.
* **User Authentication:**  Add user accounts so that multiple users can manage their own task lists.
* **Advanced Features:** Implement features like task prioritization, reminders, recurring tasks, and integration with calendar applications. 
