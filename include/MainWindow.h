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
    void onSearchStudent();

    // Course slots
    void onSortCourses();

    // Prerequisite slots
    void onFindPrerequisites();
    void onCheckCycles();

private:
    // Backend
    CollegeSystem system;
    QString currentRole;

    // Login
    bool showLoginDialog();

    // Tab creation
    QWidget* createDepartmentsTab();
    QWidget* createStudentsTab();
    QWidget* createCoursesTab();
    QWidget* createPrerequisitesTab();

    // Table refresh
    void refreshDepartmentsTable();
    void refreshStudentsTable();
    void refreshCoursesTable();

    // Styling
    void applyStyleSheet();

    // Widgets
    QTabWidget *tabWidget;

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
