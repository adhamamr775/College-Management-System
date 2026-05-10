#ifndef COURSE_H
#define COURSE_H

#include <string>
using namespace std;

class Course {
public:
    string code;
    string name;
    int capacity;

    Course();
    Course(string c, string n, int cap);

    // Getters
    string getCode() const { return code; }
    string getName() const { return name; }
    int getCapacity() const { return capacity; }
};

#endif
