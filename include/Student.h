#pragma once
#include "User.h"

class Student : public User {
public:
    int id;
    string name;
    float gpa;
    int year;
    string phone;

    // Default Constructor
    Student();

    // Parameterized Constructor (Includes login info!)
    Student(string _user, string _pass, int _id, string _name, float _gpa, int _year, string _phone);

    // Overriding the pure virtual function from User
    void display() const override;
};
