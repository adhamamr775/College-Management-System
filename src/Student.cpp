#include "../include/Student.h"

// Default Constructor
Student::Student() : User(), id(0), name(""), gpa(0.0), year(0), phone(""), departmentCode("") {}

// Parameterized Constructor (Includes login info!)
Student::Student(std::string _user, std::string _pass, int _id, std::string _name, float _gpa, int _year, std::string _phone, std::string _deptCode) 
    : User(_user, _pass, "STUDENT"), id(_id), name(_name), gpa(_gpa), year(_year), phone(_phone), departmentCode(_deptCode) {}

// Overriding the pure virtual function from User
void Student::display() const {
    std::cout << "[Student] ID: " << id << " | Name: " << name 
         << " | GPA: " << gpa << " | Year: " << year 
         << " | Dept: " << departmentCode
         << " | Phone: " << phone << std::endl;
}

void Student::addCourseRecord(const std::string& courseCode, const std::string& grade) {
    courseHistory.push_back({courseCode, grade});
}

bool Student::hasPassedPrerequisite(const std::string& courseCode) const {
    for (const auto& record : courseHistory) {
        if (record.courseCode == courseCode && record.grade != "F") {
            return true;
        }
    }
    return false;
}

bool Student::canRetakeCourse(const std::string& courseCode) const {
    for (const auto& record : courseHistory) {
        if (record.courseCode == courseCode) {
            // A or B blocks retakes
            if (record.grade == "A" || record.grade == "B") return false;
            // C, D, or F allows retake
            return true;
        }
    }
    // Never taken it, can enroll
    return true;
}
