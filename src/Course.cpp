#include "../include/Course.h"
#include <algorithm>

Course::Course() {
    code = "";
    name = "";
    capacity = 0;
    creditHours = 3;
}

Course::Course(string c, string n, int cap, int credits) {
    code = c;
    name = n;
    capacity = cap;
    creditHours = credits;
}

bool Course::isStudentEnrolled(int studentId) const {
    for (int id : enrolledStudents) {
        if (id == studentId) return true;
    }
    return false;
}

bool Course::enrollStudent(int studentId) {
    if (isStudentEnrolled(studentId)) return false;
    if ((int)enrolledStudents.size() >= capacity) return false;
    enrolledStudents.push_back(studentId);
    return true;
}

bool Course::dropStudent(int studentId) {
    auto it = std::find(enrolledStudents.begin(), enrolledStudents.end(), studentId);
    if (it != enrolledStudents.end()) {
        enrolledStudents.erase(it);
        return true;
    }
    return false;
}
