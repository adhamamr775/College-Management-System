#ifndef COLLEGESYSTEM_H
#define COLLEGESYSTEM_H

#include <vector>
#include "Department.h"
#include "Course.h"
#include "SortStrategies.h"
#include "PrerequisiteGraph.h"
using namespace std;

class CollegeSystem {
private:
    vector<Department> departments;
    vector<Course> courses;
    PrerequisiteGraph prereqGraph;

public:
    // Department management
    void addDepartment(string code, string name);
    void sortDepartmentsByName();
    void displayDepartments() const;

    // Course management
    void addCourse(string code, string name, int capacity);
    void sortCoursesByCapacity();
    void displayCourses() const;

    // Prerequisite graph
    void addPrerequisite(const string& course, const string& prerequisite);
    void displayPrerequisites(const string& courseCode) const;
    bool checkForCycles() const;
    void displayGraph() const;

    // Data initialization with mock data
    void initializeData();
};

#endif
