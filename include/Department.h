#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
using namespace std;

class Department {
public:
    string code;
    string name;

    Department(string c, string n);

    // Getters
    string getCode() const { return code; }
    string getName() const { return name; }
};

#endif
