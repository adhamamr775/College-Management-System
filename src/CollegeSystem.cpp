#include "../include/CollegeSystem.h"
#include <iostream>

void CollegeSystem::addDepartment(string code, string name) {
    departments.push_back(Department(code, name));
}

void CollegeSystem::sortDepartmentsByName() {
    MergeSortStrategy sorter;
    sorter.sortDepartments(departments);
}

void CollegeSystem::displayDepartments() const {
    for (const Department& d : departments) {
        cout << d.code << " - " << d.name << endl;
    }
}
