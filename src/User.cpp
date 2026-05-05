#include "../include/User.h"

// Default Constructor
User::User() : username(""), password(""), role("NONE") {}

// Parameterized Constructor
User::User(std::string _user, std::string _pass, std::string _role) 
    : username(_user), password(_pass), role(_role) {}

// Virtual destructor
User::~User() {}

// Login checks
bool User::checkPassword(std::string pass) const {
    return password == pass;
}

std::string User::getRole() const {
    return role;
}

std::string User::getUsername() const {
    return username;
}
