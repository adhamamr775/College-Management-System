#include "../include/CollegeSystem.h"
#include <iostream>
#include <algorithm>

// ---- Department Management ----

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

// ---- Student Management ----

void CollegeSystem::addStudent(string user, string pass, int id, string name, float gpa, int year, string phone, string deptCode) {
    students.push_back(Student(user, pass, id, name, gpa, year, phone, deptCode));
}

// ---- Instructor and Admin Management ----

void CollegeSystem::addInstructor(string user, string pass, int id, string name, string deptCode) {
    instructors.push_back(Instructor(user, pass, id, name, deptCode));
}

void CollegeSystem::addAdmin(string user, string pass) {
    admins.push_back(Admin(user, pass));
}

vector<UserInfo> CollegeSystem::getAllUsers() const {
    vector<UserInfo> allUsers;
    for (const auto& a : admins) {
        allUsers.push_back({a.getUsername(), a.getPassword(), a.getRole()});
    }
    for (const auto& i : instructors) {
        allUsers.push_back({i.getUsername(), i.getPassword(), i.getRole()});
    }
    for (const auto& s : students) {
        allUsers.push_back({s.getUsername(), s.getPassword(), s.getRole()});
    }
    return allUsers;
}

void CollegeSystem::sortStudentsByGPA() {
    SelectionSortStrategy sorter;
    sorter.sortStudents(students);
}

void CollegeSystem::displayStudents() const {
    for (const Student& s : students) {
        cout << s.id << " - " << s.name << " (GPA: " << s.gpa << ")" << endl;
    }
}

// ---- Course Management ----

void CollegeSystem::addCourse(string code, string name, int capacity, int creditHours) {
    courses.push_back(Course(code, name, capacity, creditHours));
    prereqGraph.addCourse(code);
}

void CollegeSystem::sortCoursesByCapacity() {
    BubbleSortStrategy sorter;
    sorter.sortCourses(courses);
}

void CollegeSystem::sortCoursesByEnrolledCount() {
    // Bubble sort inline or via strategy. Since BubbleSortStrategy isn't generic yet, I'll do inline for enrolled count.
    int n = courses.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (courses[j].getEnrolledCount() > courses[j + 1].getEnrolledCount()) {
                Course temp = courses[j];
                courses[j] = courses[j + 1];
                courses[j + 1] = temp;
            }
        }
    }
}

void CollegeSystem::displayCourses() const {
    for (const Course& c : courses) {
        cout << c.code << " - " << c.name << " (Capacity: " << c.capacity << ")" << endl;
    }
}

// ---- Search ----

int CollegeSystem::linearSearchDepartment(string name) {
    for (size_t i = 0; i < departments.size(); ++i) {
        if (departments[i].name == name) {
            return i;
        }
    }
    return -1; // Not found
}

int CollegeSystem::binarySearchStudent(int id) {

    // Sort students by ID first (binary search requires sorted data)
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.id < b.id;
    });

    int left = 0;
    int right = students.size() - 1;

    while (left <= right) {

        int mid = (left + right) / 2;

        if (students[mid].id == id) {
            return mid;
        }
        else if (students[mid].id < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1; // Not found
}

// ---- Enrollment ----

int CollegeSystem::getStudentCreditHours(int studentId) const {
    int total = 0;
    for (const Course& c : courses) {
        if (c.isStudentEnrolled(studentId)) {
            total += c.getCreditHours();
        }
    }
    return total;
}

int CollegeSystem::getMaxCreditHours(float gpa) const {
    if (gpa >= 3.0) return 21;
    if (gpa >= 2.0) return 18;
    return 12;
}

string CollegeSystem::enrollStudentInCourse(int studentId, const string& courseCode) {
    // Find the student
    Student* foundStudent = nullptr;
    for (auto& s : students) {
        if (s.getId() == studentId) {
            foundStudent = &s;
            break;
        }
    }
    if (!foundStudent) return "Student with ID " + to_string(studentId) + " not found.";

    // Find the course
    Course* foundCourse = nullptr;
    for (auto& c : courses) {
        if (c.getCode() == courseCode) {
            foundCourse = &c;
            break;
        }
    }
    if (!foundCourse) return "Course '" + courseCode + "' not found.";

    // Check if already enrolled
    if (foundCourse->isStudentEnrolled(studentId)) {
        return "Student is already enrolled in " + courseCode + ".";
    }

    // Check course capacity
    if (foundCourse->getEnrolledCount() >= foundCourse->getCapacity()) {
        return "Course " + courseCode + " is full (" + to_string(foundCourse->getCapacity()) + "/" + to_string(foundCourse->getCapacity()) + ").";
    }

    // Check GPA-based credit hour limit
    int currentHours = getStudentCreditHours(studentId);
    int maxHours = getMaxCreditHours(foundStudent->getGpa());
    int newTotal = currentHours + foundCourse->getCreditHours();

    if (newTotal > maxHours) {
        return "Cannot enroll: " + to_string(currentHours) + "/" + to_string(maxHours) +
               " credit hours used. Adding " + courseCode + " (" +
               to_string(foundCourse->getCreditHours()) + " hrs) would exceed the limit for GPA " +
               to_string(foundStudent->getGpa()).substr(0, 4) + ".";
    }

    // Check if can retake
    if (!foundStudent->canRetakeCourse(courseCode)) {
        return "You have already passed " + courseCode + " with a high grade and cannot retake it.";
    }

    // Check prerequisites
    vector<string> prereqs = prereqGraph.getPrerequisitesBFS(courseCode);
    for (const string& p : prereqs) {
        if (!foundStudent->hasPassedPrerequisite(p)) {
            return "Missing prerequisite: You must pass " + p + " before enrolling in " + courseCode + ".";
        }
    }

    // Enroll!
    foundCourse->enrollStudent(studentId);
    return "OK";
}

string CollegeSystem::dropStudentFromCourse(int studentId, const string& courseCode) {
    // Find the course
    Course* foundCourse = nullptr;
    for (auto& c : courses) {
        if (c.getCode() == courseCode) {
            foundCourse = &c;
            break;
        }
    }
    if (!foundCourse) return "Course not found.";

    if (!foundCourse->isStudentEnrolled(studentId)) {
        return "Student is not enrolled in this course.";
    }

    foundCourse->dropStudent(studentId);
    return "OK";
}

// ---- Prerequisite Graph ----

void CollegeSystem::addPrerequisite(const string& course, const string& prerequisite) {
    prereqGraph.addPrerequisite(course, prerequisite);
}

void CollegeSystem::displayPrerequisites(const string& courseCode) const {
    cout << "All prerequisites for " << courseCode << ": ";
    vector<string> prereqs = prereqGraph.getPrerequisitesBFS(courseCode);
    if (prereqs.empty()) {
        cout << "None" << endl;
    } else {
        for (const string& p : prereqs) {
            cout << p << " ";
        }
        cout << endl;
    }
}

bool CollegeSystem::checkForCycles() const {
    return prereqGraph.hasCycle();
}

void CollegeSystem::displayGraph() const {
    prereqGraph.displayGraph();
}

vector<string> CollegeSystem::getPrerequisitesBFS(const string& courseCode) const {
    return prereqGraph.getPrerequisitesBFS(courseCode);
}

// ---- Data Access (for GUI) ----

const vector<Department>& CollegeSystem::getDepartments() const {
    return departments;
}

const vector<Student>& CollegeSystem::getStudents() const {
    return students;
}

const vector<Instructor>& CollegeSystem::getInstructors() const {
    return instructors;
}

const vector<Admin>& CollegeSystem::getAdmins() const {
    return admins;
}

const vector<Course>& CollegeSystem::getCourses() const {
    return courses;
}

// ---- Data Initialization ----

void CollegeSystem::initializeData() {
    // Mock Departments
    addDepartment("CS", "Computer Science");
    addDepartment("BUS", "Business");
    addDepartment("IT", "Information Technology");
    addDepartment("AI", "Artificial Intelligence");

    // Mock Courses
    addCourse("CS101", "Intro to Programming", 100, 3);
    addCourse("CS201", "Data Structures", 80, 3);
    addCourse("CS301", "Algorithms", 60, 3);
    addCourse("CS401", "Machine Learning", 40, 3);
    addCourse("MATH101", "Calculus I", 120, 4);
    addCourse("MATH201", "Linear Algebra", 90, 4);

    // Mock Admins
    addAdmin("admin", "admin");

    // Mock Instructors
    addInstructor("prof_smith", "pass", 2001, "Dr. Smith", "CS");
    addInstructor("prof_jones", "pass", 2002, "Dr. Jones", "MATH");

    // Mock Students
    addStudent("alice", "pass1", 1001, "Alice Johnson", 3.8, 2, "555-0101", "CS");
    students[0].addCourseRecord("CS101", "A"); // Alice passed CS101

    addStudent("bob", "pass2", 1002, "Bob Smith", 3.2, 3, "555-0102", "BUS");
    students[1].addCourseRecord("MATH101", "C"); // Bob passed MATH101 with C (can retake)

    addStudent("charlie", "pass3", 1003, "Charlie Brown", 3.5, 1, "555-0103", "IT");
    students[2].addCourseRecord("CS101", "B");
    students[2].addCourseRecord("CS201", "F"); // Charlie failed CS201

    addStudent("diana", "pass4", 1004, "Diana Prince", 3.9, 4, "555-0104", "AI");
    students[3].addCourseRecord("CS101", "A");
    students[3].addCourseRecord("CS201", "A");
    students[3].addCourseRecord("CS301", "B");
    students[3].addCourseRecord("MATH101", "A");
    students[3].addCourseRecord("MATH201", "A"); // Diana passed everything except CS401

    addStudent("eve", "pass5", 1005, "Eve Adams", 2.8, 2, "555-0105", "CS");

    // Mock Prerequisites
    // CS201 requires CS101
    addPrerequisite("CS201", "CS101");

    // CS301 requires CS201 (and transitively CS101)
    addPrerequisite("CS301", "CS201");

    // CS401 requires CS301 and MATH201
    addPrerequisite("CS401", "CS301");
    addPrerequisite("CS401", "MATH201");

    // MATH201 requires MATH101
    addPrerequisite("MATH201", "MATH101");
}

void CollegeSystem::sortStudentsById() {
    MergeSortStrategy sorter;
    sorter.sortStudentsById(students);
}
