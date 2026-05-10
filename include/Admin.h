#pragma once
#include "User.h"

class Admin : public User {
public:
    // Default Constructor
    Admin();

    // Parameterized Constructor
    Admin(string _user, string _pass);

    // Overriding the pure virtual function from User
    void display() const override;
};
