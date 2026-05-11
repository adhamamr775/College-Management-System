# College Management System (CMS) 🎓

A robust, Object-Oriented desktop application built with **C++** and the **Qt Framework**. This system manages university entities (Departments, Courses, and Students), enforces role-based access control, and implements advanced data structures and algorithms to handle sorting, searching, and complex course prerequisite mapping.

## ✨ Key Features

*   **Role-Based Access Control (RBAC):** Secure login system distinguishing between `Instructors/Admins` and `Students`, ensuring users only access authorized dashboards.
*   **Entity Management:** Add, view, and manage Departments, Courses, and Students.
*   **Dynamic Sorting:** Sort entities using three distinct algorithms, cleanly implemented via the **Strategy Design Pattern** (SOLID Principles).
*   **High-Speed Searching:** Quickly locate students by ID using Binary Search, and departments by Name using Linear Search.
*   **Prerequisite Graphing:** View all direct and indirect course prerequisites instantly.
*   **Automated Cycle Detection:** Prevents admins from creating impossible prerequisite loops (e.g., Course A requires Course B, which requires Course A).
*   **Interactive GUI:** A clean, user-friendly graphical interface built with Qt.

## 🧠 Data Structures & Algorithms

This project heavily utilizes standard C++ containers and custom algorithmic implementations to ensure performance and correctness.

### Data Structures Used:
*   `std::vector`: Dynamic storage for all primary entities (Students, Courses, Departments).
*   `std::unordered_map`: Used as an adjacency list to represent the Directed Graph of course prerequisites.
*   `std::queue`: Utilized for Breadth-First traversal.
*   `std::stack` / Recursion Stack: Utilized for Depth-First traversal.

### Algorithms Implemented:
*   **Selection Sort:** $O(n^2)$ - Used to sort Students by GPA.
*   **Bubble Sort:** $O(n^2)$ - Used to sort Courses by Enrollment capacity.
*   **Merge Sort:** $O(n \log n)$ - Used to sort Departments alphabetically by Name.
*   **Linear Search:** $O(n)$ - Used to retrieve specific Departments.
*   **Binary Search:** $O(\log n)$ - Used to retrieve specific Students (includes auto-resorting fallback if the primary vector is currently sorted by a different key).
*   **Breadth-First Search (BFS):** Traverses the prerequisite graph layer-by-layer to display all required courses to the user.
*   **Depth-First Search (DFS):** Explores graph paths deeply to detect "back-edges," successfully identifying and preventing prerequisite cycles.

## 🏗️ Software Architecture

This project adheres to **SOLID** Object-Oriented Design principles:
*   **Single Responsibility Principle:** Graph logic is isolated in a dedicated `PrerequisiteGraph` class, while Authentication and Data management are handled separately.
*   **Open/Closed Principle:** Sorting logic utilizes the `ISortStrategy` interface. New sorting algorithms can be added without modifying the core `CollegeSystem` controller.
*   **Dependency Inversion:** The Qt GUI (`MainWindow`) communicates with the underlying data strictly through the `CollegeSystem` API, decoupling the interface from the database implementation.

### Tasks:
1. Defining Main classes (department, courses, students)
Where:
Department: include/Department.h (declarations) & src/Department.cpp (implementations)
Course: include/Course.h (declarations) & src/Course.cpp (implementations)
Student: include/Student.h & src/Student.cpp (Already done)
Details: Define the attributes (e.g., course name, department capacity) and basic methods including constructors and getters/setters. (DONE)
2. Merge sort for department by name
Where: include/SortStrategies.h & src/CollegeSystem.cpp
Details: You have a dedicated file named SortStrategies.h. It is best practice to implement your sorting algorithms (like Merge Sort) there as static functions or templates. You will then call this sort function in src/CollegeSystem.cpp where your array or vector of departments is actually stored.
3. Selection sort for students by GPA
Where: include/SortStrategies.h & src/CollegeSystem.cpp
Details: Similar to Merge Sort, implement the general Selection Sort logic inside SortStrategies.h. Then, use it inside CollegeSystem.cpp to sort the system's list of Student objects.
4. Bubble Sort courses by number
Where: include/SortStrategies.h & src/CollegeSystem.cpp
Details: Implement the Bubble Sort algorithm in SortStrategies.h and apply it to the list of Course objects inside CollegeSystem.cpp.
5. Linear search for department by name or binary search for student by id
Where: src/CollegeSystem.cpp
Details: Implement the linearSearchDepartment(string name) and binarySearchStudent(int id) methods directly inside the CollegeSystem class. It manages the collections of data, making it the perfect place to query them.
6. Graph + BFS + Data initialization
Where:
Graph + BFS: include/PrerequisiteGraph.h & src/PrerequisiteGraph.cpp
Data Initialization: src/main.cpp or src/CollegeSystem.cpp
Details: The graph data structure (using adjacency lists/matrices) and the Breadth-First Search (BFS) traversal logic to find course prerequisites should be written in PrerequisiteGraph. The actual mock data creation (creating objects and inserting them into the system) should be handled at the start of your program in main.cppmain.cppor inside a dedicated initialization method inCollegeSystem.


---
*Developed for [Course Name/Code] at [University Name]*
