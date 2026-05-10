cmake_minimum_required(VERSION 3.16)
project(CollegeManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's Meta-Object Compiler (MOC)
set(CMAKE_AUTOMOC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ---- Shared source files (backend) ----
set(BACKEND_SOURCES
    src/CollegeSystem.cpp
    src/Department.cpp
    src/Course.cpp
    src/User.cpp
    src/Admin.cpp
    src/Student.cpp
    src/Instructor.cpp
    src/PrerequisiteGraph.cpp
)

# ---- Console target ----
add_executable(cms_console
    src/main.cpp
    ${BACKEND_SOURCES}
)
target_include_directories(cms_console PRIVATE include)

# ---- GUI target ----
add_executable(cms_gui
    src/gui_main.cpp
    src/MainWindow.cpp
    include/MainWindow.h
    ${BACKEND_SOURCES}
)
target_include_directories(cms_gui PRIVATE include)
target_link_libraries(cms_gui PRIVATE Qt6::Widgets)
