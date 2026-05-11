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

    // Display all students
    cout << "=== Students ===" << endl;
    system.displayStudents();
    cout << endl;

    // Sort students by GPA (Selection Sort) and display
    system.sortStudentsByGPA();
    cout << "=== Students (Sorted by GPA) ===" << endl;
    system.displayStudents();
    cout << endl;

    // Linear Search for department by name
    cout << "=== Search ===" << endl;
    int deptIndex = system.linearSearchDepartment("Computer Science");
    if (deptIndex != -1) {
        cout << "Department 'Computer Science' found at index " << deptIndex << endl;
    } else {
        cout << "Department 'Computer Science' not found" << endl;
    }

    deptIndex = system.linearSearchDepartment("Physics");
    if (deptIndex != -1) {
        cout << "Department 'Physics' found at index " << deptIndex << endl;
    } else {
        cout << "Department 'Physics' not found" << endl;
    }

    // Binary Search for student by ID
    int studentIndex = system.binarySearchStudent(1003);
    if (studentIndex != -1) {
        cout << "Student with ID 1003 found at index " << studentIndex << endl;
    } else {
        cout << "Student with ID 1003 not found" << endl;
    }

    studentIndex = system.binarySearchStudent(9999);
    if (studentIndex != -1) {
        cout << "Student with ID 9999 found at index " << studentIndex << endl;
    } else {
        cout << "Student with ID 9999 not found" << endl;
    }
    cout << endl;

    // Display the prerequisite graph
    cout << "=== Prerequisite Graph ===" << endl;
    system.displayGraph();
    cout << endl;

    // Show all prerequisites for CS401
    cout << "=== Prerequisite Lookup ===" << endl;
    system.displayPrerequisites("CS401");
    system.displayPrerequisites("CS301");
    system.displayPrerequisites("CS101");
    cout << endl;

    // Cycle detection
    cout << "=== Cycle Detection ===" << endl;
    if (system.checkForCycles()) {
        cout << "WARNING: Cycle detected in prerequisites!" << endl;
    }
    else {
        cout << "No cycles detected. Prerequisites are valid." << endl;
    }

    return 0;
}
