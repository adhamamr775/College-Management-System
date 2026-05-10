#ifndef COLLEGESYSTEM_H
#define COLLEGESYSTEM_H

#include <vector>
#include "Department.h"
#include "Course.h"
#include "Student.h"
#include "SortStrategies.h"
#include "PrerequisiteGraph.h"
using namespace std;

class CollegeSystem {
private:
    vector<Department> departments;
    vector<Course> courses;
    vector<Student> students;
    PrerequisiteGraph prereqGraph;

public:
    // Department management
    void addDepartment(string code, string name);
    void sortDepartmentsByName();
    void displayDepartments() const;

    // Student management
    void addStudent(string user, string pass, int id, string name, float gpa, int year, string phone);
    void sortStudentsByGPA();
    void displayStudents() const;

    // Course management
    void addCourse(string code, string name, int capacity);
    void sortCoursesByCapacity();
    void displayCourses() const;

    // Binary search
    int binarySearchDepartment(string name);
    int binarySearchStudent(int id);

    // Prerequisite graph
    void addPrerequisite(const string& course, const string& prerequisite);
    void displayPrerequisites(const string& courseCode) const;
    bool checkForCycles() const;
    void displayGraph() const;
    vector<string> getPrerequisitesBFS(const string& courseCode) const;

    // Data access (for GUI)
    const vector<Department>& getDepartments() const;
    const vector<Student>& getStudents() const;
    const vector<Course>& getCourses() const;

    // Data initialization with mock data
    void initializeData();
};

#endif
