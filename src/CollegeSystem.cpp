#include "../include/CollegeSystem.h"
#include <iostream>

// ---- Department Management ----

void CollegeSystem::addDepartment(string code, string name) {
    departments.push_back(Department(code, name));
}

void CollegeSystem::sortDepartmentsByName() {
    MergeSortStrategy sorter;
    sorter.sortDepartments(departments);
}

void CollegeSystem::displayDepartments() const {
    for (const Department& d : departments) {
        cout << d.code << " - " << d.name << endl;
    }
}

// ---- Course Management ----

void CollegeSystem::addCourse(string code, string name, int capacity) {
    courses.push_back(Course(code, name, capacity));
    prereqGraph.addCourse(code);
}

void CollegeSystem::displayCourses() const {
    for (const Course& c : courses) {
        cout << c.code << " - " << c.name << " (Capacity: " << c.capacity << ")" << endl;
    }
}

// ---- Prerequisite Graph ----

void CollegeSystem::addPrerequisite(const string& course, const string& prerequisite) {
    prereqGraph.addPrerequisite(course, prerequisite);
}

void CollegeSystem::displayPrerequisites(const string& courseCode) const {
    cout << "All prerequisites for " << courseCode << " (BFS): ";
    vector<string> prereqs = prereqGraph.getPrerequisitesBFS(courseCode);

    if (prereqs.empty()) {
        cout << "None" << endl;
    }
    else {
        for (const string& p : prereqs) {
            cout << p << " ";
        }
        cout << endl;
    }
}

bool CollegeSystem::checkForCycles() const {
    return prereqGraph.hasCycle();
}

void CollegeSystem::displayGraph() const {
    prereqGraph.displayGraph();
}

// ---- Data Initialization ----

void CollegeSystem::initializeData() {
    // Mock Departments
    addDepartment("CS", "Computer Science");
    addDepartment("BUS", "Business");
    addDepartment("IT", "Information Technology");
    addDepartment("AI", "Artificial Intelligence");

    // Mock Courses
    addCourse("CS101", "Intro to Programming", 100);
    addCourse("CS201", "Data Structures", 80);
    addCourse("CS301", "Algorithms", 60);
    addCourse("CS401", "Machine Learning", 40);
    addCourse("MATH101", "Calculus I", 120);
    addCourse("MATH201", "Linear Algebra", 90);

    // Mock Prerequisites
    // CS201 requires CS101
    addPrerequisite("CS201", "CS101");

    // CS301 requires CS201 (and transitively CS101)
    addPrerequisite("CS301", "CS201");

    // CS401 requires CS301 and MATH201
    addPrerequisite("CS401", "CS301");
    addPrerequisite("CS401", "MATH201");

    // MATH201 requires MATH101
    addPrerequisite("MATH201", "MATH101");
}
