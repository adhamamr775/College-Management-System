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

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    float getGpa() const { return gpa; }
    int getYear() const { return year; }
    string getPhone() const { return phone; }

    // Overriding the pure virtual function from User
    void display() const override;
};
