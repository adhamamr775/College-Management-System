#ifndef COLLEGESYSTEM_H
#define COLLEGESYSTEM_H

#include <vector>
#include "Department.h"
#include "SortStrategies.h"
using namespace std;

class CollegeSystem {
private:
    vector<Department> departments;

public:
    void addDepartment(string code, string name);
    void sortDepartmentsByName();
    void displayDepartments() const;
};

#endif
