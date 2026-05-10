#pragma once
#include "User.h"
#include <vector>

class Student : public User {
public:
    int id;
    string name;
    float gpa;
    int year;
    string phone;
    std::string departmentCode;
    
    struct CourseRecord {
        std::string courseCode;
        std::string grade;
    };
    std::vector<CourseRecord> courseHistory;

    // Default Constructor
    Student();

    // Parameterized Constructor (Includes login info!)
    Student(string _user, string _pass, int _id, string _name, float _gpa, int _year, string _phone, string _deptCode = "");

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    float getGpa() const { return gpa; }
    int getYear() const { return year; }
    string getPhone() const { return phone; }
    std::string getDepartmentCode() const { return departmentCode; }
    const std::vector<CourseRecord>& getCourseHistory() const { return courseHistory; }

    // Academic History Logic
    void addCourseRecord(const std::string& courseCode, const std::string& grade);
    bool hasPassedPrerequisite(const std::string& courseCode) const;
    bool canRetakeCourse(const std::string& courseCode) const;

    // Overriding the pure virtual function from User
    void display() const override;
};
