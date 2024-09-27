#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <ncurses.h>
using namespace std;
class Task {
public:
    int id;
    string description;
    bool completed;

    Task(int id, string description, bool completed = false)
        : id(id), description(description), completed(completed) {}
};

vector<Task> tasks;
int selectedIndex = 0;

void addTask();
void listTasks();
void markTaskCompleted();
void deleteTask();
void saveTasks();
void loadTasks();
void mainMenu();
void drawTasks();
void updateTaskIDs();

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    loadTasks();
    mainMenu();

    endwin();
    saveTasks();
    return 0;
}

void mainMenu() {
    int ch;
    while (true) {
        clear();
        drawTasks();
        ch = getch();
        
        switch (ch) {
            case 3:  // Ctrl+C to exit (used to simulate endwin for demonstration)
                endwin();
                return;
            case 10:  // Enter key
                markTaskCompleted();
                break;
            case 4:  // Ctrl + D
                deleteTask();
                break;
            case 1:  // Ctrl + A
                addTask();
                break;
            case 5:  // Ctrl + E
                markTaskCompleted();
                break;
            case 24:  // Ctrl + X
                saveTasks();
                return;
            case KEY_UP:
                if (selectedIndex > 0) selectedIndex--;
                break;
            case KEY_DOWN:
                if (selectedIndex < tasks.size() - 1) selectedIndex++;
                break;
            default:
                mvprintw(LINES - 1, 0, "Press Ctrl+X to quit.");
        }
    }
}

void drawTasks() {
    for (size_t i = 0; i < tasks.size(); i++) {
        if (i == selectedIndex) {
            attron(A_REVERSE);
        }
        mvprintw(i * 2, 0, "ID: %d | %s [%s]", tasks[i].id, tasks[i].description.c_str(), tasks[i].completed ? "X" : " ");
        if (i == selectedIndex) {
            attroff(A_REVERSE);
        }
    }
    mvprintw(LINES - 1, 0, "Ctrl+A: Add Task, Ctrl+D: Delete Task, Ctrl+E: Toggle Status, Ctrl+X: Save & Quit");
}

void addTask() {
    static int idCounter = tasks.empty() ? 1 : tasks.back().id + 1;
    char description[256];
    echo();
    mvprintw(LINES - 2, 0, "Enter task description: ");
    getnstr(description, 255);
    tasks.push_back(Task(idCounter++, description));
    noecho();
}

void markTaskCompleted() {
    if (selectedIndex >= 0 && selectedIndex < tasks.size()) {
        tasks[selectedIndex].completed = !tasks[selectedIndex].completed;
    }
}

void deleteTask() {
    if (selectedIndex >= 0 && selectedIndex < tasks.size()) {
        tasks.erase(tasks.begin() + selectedIndex);
        updateTaskIDs();
        if (selectedIndex >= tasks.size()) {
            selectedIndex = tasks.size() - 1;
        }
    }
}

void updateTaskIDs() {
    for (size_t i = 0; i < tasks.size(); ++i) {
        tasks[i].id = i + 1;
    }
}

void saveTasks() {
    ofstream file("tasks.txt");
    for (const auto &task : tasks) {
        file << task.id << ' ' << task.completed << ' ' << task.description << '\n';
    }
}

void loadTasks() {
    ifstream file("tasks.txt");
    if (file) {
        int id;
        bool completed;
        string description;
        while (file >> id >> completed) {
            getline(file, description);
            if (!description.empty() && description[0] == ' ') {
                description = description.substr(1); 
            }
            tasks.push_back(Task(id, description, completed));
        }
        updateTaskIDs();
    }
}
