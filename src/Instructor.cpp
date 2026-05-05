#include "../include/Instructor.h"

// Default Constructor
Instructor::Instructor() : User(), id(0), name(""), departmentCode("") {}

// Parameterized Constructor
Instructor::Instructor(std::string _user, std::string _pass, int _id, std::string _name, std::string _deptCode)
    : User(_user, _pass, "INSTRUCTOR"), id(_id), name(_name), departmentCode(_deptCode) {}

// Overriding the pure virtual function from User
void Instructor::display() const {
    std::cout << "[Instructor] ID: " << id << " | Name: " << name 
         << " | Dept: " << departmentCode << std::endl;
}
