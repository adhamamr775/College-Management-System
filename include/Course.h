#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
using namespace std;

class Course {
public:
    string code;
    string name;
    int capacity;
    int creditHours;
    vector<int> enrolledStudents;

    Course();
    Course(string c, string n, int cap, int credits);

    // Getters
    string getCode() const { return code; }
    string getName() const { return name; }
    int getCapacity() const { return capacity; }
    int getCreditHours() const { return creditHours; }
    int getEnrolledCount() const { return enrolledStudents.size(); }
    const vector<int>& getEnrolledStudents() const { return enrolledStudents; }

    // Enrollment
    bool isStudentEnrolled(int studentId) const;
    bool enrollStudent(int studentId);
    bool dropStudent(int studentId);
};

#endif
