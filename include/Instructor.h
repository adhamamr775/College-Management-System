#pragma once
#include "User.h"

class Instructor : public User {
public:
    int id;
    string name;
    string departmentCode;

    // Default Constructor
    Instructor();

    // Parameterized Constructor
    Instructor(string _user, string _pass, int _id, string _name, string _deptCode);

    // Overriding the pure virtual function from User
    void display() const override;
};
