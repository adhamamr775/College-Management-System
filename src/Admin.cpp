#include "../include/Admin.h"

// Default Constructor
Admin::Admin() : User() {}

// Parameterized Constructor
Admin::Admin(string _user, string _pass) : User(_user, _pass, "ADMIN") {}

// Overriding the pure virtual function from User
void Admin::display() const {
    cout << "[Admin] Username: " << username << endl;
}
