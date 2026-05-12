#ifndef COLLEGESYSTEM_H
#define COLLEGESYSTEM_H

#include <vector>
#include "Department.h"
#include "Course.h"
#include "Student.h"
#include "Instructor.h"
#include "Admin.h"
#include "SortStrategies.h"
#include "PrerequisiteGraph.h"
using namespace std;

struct UserInfo {
    string username;
    string password;
    string role;
};

class CollegeSystem {
private:
    vector<Department> departments;
    vector<Course> courses;
    vector<Student> students;
    vector<Instructor> instructors;
    vector<Admin> admins;
    PrerequisiteGraph prereqGraph;

public:
    // Department management
    void addDepartment(string code, string name);
    void sortDepartmentsByName();
    void displayDepartments() const;

    // Student management
    void addStudent(string user, string pass, int id, string name, float gpa, int year, string phone, string deptCode = "");
    void sortStudentsByGPA();
    void sortStudentsById();
    void displayStudents() const;

    // Instructor and Admin management
    void addInstructor(string user, string pass, int id, string name, string deptCode);
    void addAdmin(string user, string pass);
    vector<UserInfo> getAllUsers() const;

    // Course management
    void addCourse(string code, string name, int capacity, int creditHours);
    void sortCoursesByCapacity();
    void sortCoursesByEnrolledCount();
    void displayCourses() const;

    // Search
    int linearSearchDepartment(string name);
    int binarySearchStudent(int id);

    // Enrollment
    int getStudentCreditHours(int studentId) const;
    int getMaxCreditHours(float gpa) const;
    string enrollStudentInCourse(int studentId, const string& courseCode);
    string dropStudentFromCourse(int studentId, const string& courseCode);

    // Prerequisite graph
    void addPrerequisite(const string& course, const string& prerequisite);
    void displayPrerequisites(const string& courseCode) const;
    bool checkForCycles() const;
    void displayGraph() const;
    vector<string> getPrerequisitesBFS(const string& courseCode) const;

    // Data access (for GUI)
    const vector<Department>& getDepartments() const;
    const vector<Student>& getStudents() const;
    const vector<Instructor>& getInstructors() const;
    const vector<Admin>& getAdmins() const;
    const vector<Course>& getCourses() const;

    // Data initialization with mock data
    void initializeData();
};

#endif
