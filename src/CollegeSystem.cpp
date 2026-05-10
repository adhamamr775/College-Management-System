#include "../include/CollegeSystem.h"
#include <iostream>
#include <algorithm>

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

// ---- Student Management ----

void CollegeSystem::addStudent(string user, string pass, int id, string name, float gpa, int year, string phone) {
    students.push_back(Student(user, pass, id, name, gpa, year, phone));
}

void CollegeSystem::sortStudentsByGPA() {
    SelectionSortStrategy sorter;
    sorter.sortStudents(students);
}

void CollegeSystem::displayStudents() const {
    for (const Student& s : students) {
        cout << s.id << " - " << s.name << " (GPA: " << s.gpa << ")" << endl;
    }
}

// ---- Course Management ----

void CollegeSystem::addCourse(string code, string name, int capacity) {
    courses.push_back(Course(code, name, capacity));
    prereqGraph.addCourse(code);
}

void CollegeSystem::sortCoursesByCapacity() {
    BubbleSortStrategy sorter;
    sorter.sortCourses(courses);
}

void CollegeSystem::displayCourses() const {
    for (const Course& c : courses) {
        cout << c.code << " - " << c.name << " (Capacity: " << c.capacity << ")" << endl;
    }
}

// ---- Binary Search ----

int CollegeSystem::binarySearchDepartment(string name) {

    // Sort departments by name first (binary search requires sorted data)
    sortDepartmentsByName();

    int left = 0;
    int right = departments.size() - 1;

    while (left <= right) {

        int mid = (left + right) / 2;

        if (departments[mid].name == name) {
            return mid;
        }
        else if (departments[mid].name < name) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1; // Not found
}

int CollegeSystem::binarySearchStudent(int id) {

    // Sort students by ID first (binary search requires sorted data)
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.id < b.id;
    });

    int left = 0;
    int right = students.size() - 1;

    while (left <= right) {

        int mid = (left + right) / 2;

        if (students[mid].id == id) {
            return mid;
        }
        else if (students[mid].id < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1; // Not found
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

vector<string> CollegeSystem::getPrerequisitesBFS(const string& courseCode) const {
    return prereqGraph.getPrerequisitesBFS(courseCode);
}

// ---- Data Access (for GUI) ----

const vector<Department>& CollegeSystem::getDepartments() const {
    return departments;
}

const vector<Student>& CollegeSystem::getStudents() const {
    return students;
}

const vector<Course>& CollegeSystem::getCourses() const {
    return courses;
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

    // Mock Students
    addStudent("alice", "pass1", 1001, "Alice Johnson", 3.8, 2, "555-0101");
    addStudent("bob", "pass2", 1002, "Bob Smith", 3.2, 3, "555-0102");
    addStudent("charlie", "pass3", 1003, "Charlie Brown", 3.5, 1, "555-0103");
    addStudent("diana", "pass4", 1004, "Diana Prince", 3.9, 4, "555-0104");
    addStudent("eve", "pass5", 1005, "Eve Adams", 2.8, 2, "555-0105");

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

