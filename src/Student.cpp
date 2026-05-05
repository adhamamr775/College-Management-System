#include "../include/Student.h"

// Default Constructor
Student::Student() : User(), id(0), name(""), gpa(0.0), year(0), phone("") {}

// Parameterized Constructor (Includes login info!)
Student::Student(std::string _user, std::string _pass, int _id, std::string _name, float _gpa, int _year, std::string _phone) 
    : User(_user, _pass, "STUDENT"), id(_id), name(_name), gpa(_gpa), year(_year), phone(_phone) {}

// Overriding the pure virtual function from User
void Student::display() const {
    std::cout << "[Student] ID: " << id << " | Name: " << name 
         << " | GPA: " << gpa << " | Year: " << year 
         << " | Phone: " << phone << std::endl;
}
