#include <iostream>
#include "../include/CollegeSystem.h"
using namespace std;

int main() {
    CollegeSystem system;

    // Initialize mock data
    system.initializeData();

    // Display all departments
    cout << "=== Departments ===" << endl;
    system.displayDepartments();
    cout << endl;

    // Sort departments by name and display
    system.sortDepartmentsByName();
    cout << "=== Departments (Sorted by Name) ===" << endl;
    system.displayDepartments();
    cout << endl;

    // Display all courses
    cout << "=== Courses ===" << endl;
    system.displayCourses();
    cout << endl;

    // Display the prerequisite graph
    cout << "=== Prerequisite Graph ===" << endl;
    system.displayGraph();
    cout << endl;

    // BFS: Show all prerequisites for CS401
    cout << "=== BFS Prerequisite Lookup ===" << endl;
    system.displayPrerequisites("CS401");
    system.displayPrerequisites("CS301");
    system.displayPrerequisites("CS101");
    cout << endl;

    // Cycle detection
    cout << "=== Cycle Detection (DFS) ===" << endl;
    if (system.checkForCycles()) {
        cout << "WARNING: Cycle detected in prerequisites!" << endl;
    }
    else {
        cout << "No cycles detected. Prerequisites are valid." << endl;
    }

    return 0;
}
