#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDialog>
#include <QHeaderView>
#include <QSpinBox>
#include <QComboBox>
#include "../include/CollegeSystem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // Department slots
    void onSortDepartments();
    void onSearchDepartment();

    // Student slots
    void onSortStudents();
    void onSortStudentsById();
    void onSearchStudent();

    // User slots
    void onAddUser();

    // Course slots
    void onSortCourses();
    void onSortCoursesByEnrollment();
    void onEnrollStudent();

    // Prerequisite slots
    void onFindPrerequisites();
    void onCheckCycles();

    // Session slots
    void onLogout();

private:
    // Backend
    CollegeSystem system;
    QString currentRole;
    int loggedInStudentId;

    // Login & UI
    bool showLoginDialog();
    void setupMainUI();

    // Tab creation
    QWidget* createUsersTab();
    QWidget* createDepartmentsTab();
    QWidget* createStudentsTab();
    QWidget* createCoursesTab();
    QWidget* createPrerequisitesTab();
    QWidget* createMyCoursesTab();
    QWidget* createAcademicHistoryTab();

    // Table refresh
    void refreshUsersTable();
    void refreshDepartmentsTable();
    void refreshStudentsTable();
    void refreshCoursesTable();
    void refreshMyCoursesTable();
    void refreshAcademicHistoryTable();

    // Course view slot
    void onViewEnrolledStudents();
    void onDropCourse();

    // Styling
    void applyStyleSheet();

    // Widgets
    QTabWidget *tabWidget;

    // Users tab
    QTableWidget *usersTable;

    // My Courses tab
    QTableWidget *myCoursesTable;
    
    // Academic History tab
    QTableWidget *academicHistoryTable;
    QLabel *myCreditsLabel;

    // Departments tab
    QTableWidget *deptTable;
    QLineEdit *deptSearchInput;

    // Students tab
    QTableWidget *studentTable;
    QLineEdit *studentSearchInput;

    // Courses tab
    QTableWidget *courseTable;

    // Prerequisites tab
    QLineEdit *prereqCourseInput;
    QTextEdit *prereqResultText;
};

#endif
