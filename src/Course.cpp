#include "../include/Course.h"

Course::Course() {
    code = "";
    name = "";
    capacity = 0;
}

Course::Course(string c, string n, int cap) {
    code = c;
    name = n;
    capacity = cap;
}
