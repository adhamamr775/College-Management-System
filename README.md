# College Management System (CMS) 🎓

A robust, Object-Oriented desktop application built with **C++** and the **Qt Framework**. This system manages university entities (Departments, Courses, and Students), enforces role-based access control, and implements advanced data structures and algorithms to handle sorting, searching, and complex course prerequisite mapping.

## ✨ Key Features

*   **Role-Based Access Control (RBAC):** Secure login system distinguishing between `Instructors/Admins` and `Students`, ensuring users only access authorized dashboards.
*   **Entity Management:** Add, view, and manage Departments, Courses, and Students.
*   **Dynamic Sorting:** Sort entities using three distinct algorithms, cleanly implemented via the **Strategy Design Pattern** (SOLID Principles).
*   **High-Speed Searching:** Quickly locate students by ID or departments by Name using Binary Search.
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
*   **Binary Search:** $O(\log n)$ - Used to retrieve specific Students or Departments (includes auto-resorting fallback if the primary vector is currently sorted by a different key).
*   **Breadth-First Search (BFS):** Traverses the prerequisite graph layer-by-layer to display all required courses to the user.
*   **Depth-First Search (DFS):** Explores graph paths deeply to detect "back-edges," successfully identifying and preventing prerequisite cycles.

## 🏗️ Software Architecture

This project adheres to **SOLID** Object-Oriented Design principles:
*   **Single Responsibility Principle:** Graph logic is isolated in a dedicated `PrerequisiteGraph` class, while Authentication and Data management are handled separately.
*   **Open/Closed Principle:** Sorting logic utilizes the `ISortStrategy` interface. New sorting algorithms can be added without modifying the core `CollegeSystem` controller.
*   **Dependency Inversion:** The Qt GUI (`MainWindow`) communicates with the underlying data strictly through the `CollegeSystem` API, decoupling the interface from the database implementation.

## 🚀 Getting Started

### Prerequisites
*   **C++ Compiler:** GCC, Clang, or MSVC (C++17 or higher recommended).
*   **Qt Framework:** Qt 5 or Qt 6 installed.
*   **Qt Creator:** Recommended IDE for managing UI files (`.ui`) and the `.pro` or `CMakeLists.txt` build system.

### Build and Run
1.  Clone this repository.
    ```bash
    git clone [https://github.com/yourusername/CollegeManagementSystem.git](https://github.com/yourusername/CollegeManagementSystem.git)
    ```
2.  Open the project in **Qt Creator** by selecting the `CMakeLists.txt` or `.pro` file.
3.  Configure the project for your local kit (Desktop).
4.  Build the project (`Ctrl+B` or `Cmd+B`).
5.  Run the application (`Ctrl+R` or `Cmd+R`).
6.  *Note: The system initializes with dummy data automatically upon startup so you can test features immediately.*

## 📸 Screenshots
*(Add screenshots of your Qt GUI here once the project is finished!)*
*   *Login Screen*
*   *Admin Dashboard*
*   *Sorted Student Table*
*   *Prerequisite Graph Results*

---
*Developed for [Course Name/Code] at [University Name]*
