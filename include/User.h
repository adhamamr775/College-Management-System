#pragma once
#include <string>
#include <iostream>

using namespace std;

class User {
protected:
    string username;
    string password;
    string role; // "STUDENT" or "INSTRUCTOR"

public:
    // Default Constructor
    User();
    
    // Parameterized Constructor
    User(string _user, string _pass, string _role);

    // Virtual destructor (Best practice for base classes)
    virtual ~User();

    // Pure virtual function: Forces all child classes to write their own display()
    virtual void display() const = 0; 

    // Login checks
    bool checkPassword(string pass) const;

    string getRole() const;
    string getUsername() const;
    string getPassword() const;
};
