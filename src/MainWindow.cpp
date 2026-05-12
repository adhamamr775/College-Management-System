#include "../include/MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QStackedWidget>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), loggedInStudentId(-1) {
    // Initialize data
    system.initializeData();

    // Apply stylesheet
    applyStyleSheet();

    // Show login dialog
    if (!showLoginDialog()) {
        QTimer::singleShot(0, qApp, &QApplication::quit);
        return;
    }

    setupMainUI();
}

void MainWindow::setupMainUI() {
    // Window setup
    setWindowTitle("College Management System");
    setMinimumSize(950, 650);
    resize(1100, 750);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // Header row with logout
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *headerLabel = new QLabel("🎓 College Management System");
    headerLabel->setObjectName("headerLabel");
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    QPushButton *logoutBtn = new QPushButton("🚪 Logout");
    logoutBtn->setObjectName("warningBtn");
    logoutBtn->setFixedHeight(36);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);
    headerLayout->addWidget(logoutBtn);
    mainLayout->addLayout(headerLayout);

    // Role indicator
    QLabel *roleLabel = new QLabel("Logged in as: " + currentRole);
    roleLabel->setObjectName("roleLabel");
    mainLayout->addWidget(roleLabel);

    // Tab widget
    tabWidget = new QTabWidget();
    tabWidget->setObjectName("mainTabs");

    if (currentRole == "Admin" || currentRole == "Instructor") {
        tabWidget->addTab(createUsersTab(), "👥 Users");
        tabWidget->addTab(createStudentsTab(), "🎓 Students");
    } else if (currentRole == "Student") {
        tabWidget->addTab(createMyCoursesTab(), "🎓 My Courses");
        tabWidget->addTab(createAcademicHistoryTab(), "📜 Academic History");
    }
    tabWidget->addTab(createDepartmentsTab(), "📁 Departments");
    tabWidget->addTab(createCoursesTab(), "📚 Courses");
    tabWidget->addTab(createPrerequisitesTab(), "🔗 Prerequisites");

    mainLayout->addWidget(tabWidget);

    // Populate tables
    refreshDepartmentsTable();
    if (currentRole == "Admin" || currentRole == "Instructor") {
        refreshUsersTable();
        refreshStudentsTable();
    } else if (currentRole == "Student") {
        refreshMyCoursesTable();
        refreshAcademicHistoryTable();
    }
    refreshCoursesTable();
}

// ==================== LOGIN ====================

bool MainWindow::showLoginDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Login — College Management System");
    dialog.setFixedSize(420, 300);
    dialog.setObjectName("loginDialog");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(14);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *titleLabel = new QLabel("🎓 CMS Login");
    titleLabel->setObjectName("loginTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    QLineEdit *usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Enter username...");
    usernameInput->setObjectName("loginInput");
    formLayout->addRow("Username:", usernameInput);

    QLineEdit *passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Enter password...");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setObjectName("loginInput");
    formLayout->addRow("Password:", passwordInput);

    layout->addLayout(formLayout);

    QLabel *hintLabel = new QLabel("Hint: alice/pass1 (Student) or admin/admin (Instructor)");
    hintLabel->setObjectName("hintLabel");
    hintLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(hintLabel);

    QPushButton *loginBtn = new QPushButton("Login");
    loginBtn->setObjectName("loginBtn");
    loginBtn->setFixedHeight(40);
    layout->addWidget(loginBtn);

    bool accepted = false;

    connect(loginBtn, &QPushButton::clicked, [&]() {
        QString user = usernameInput->text().trimmed();
        QString pass = passwordInput->text().trimmed();

        // Check against mock students
        for (const auto& s : system.getStudents()) {
            if (QString::fromStdString(s.getUsername()) == user &&
                s.checkPassword(pass.toStdString())) {
                currentRole = "Student";
                loggedInStudentId = s.id;
                accepted = true;
                dialog.accept();
                return;
            }
        }

        // Check against instructors
        for (const auto& i : system.getInstructors()) {
            if (QString::fromStdString(i.getUsername()) == user &&
                i.checkPassword(pass.toStdString())) {
                currentRole = "Instructor";
                accepted = true;
                dialog.accept();
                return;
            }
        }

        // Check against admins
        for (const auto& a : system.getAdmins()) {
            if (QString::fromStdString(a.getUsername()) == user &&
                a.checkPassword(pass.toStdString())) {
                currentRole = "Admin";
                accepted = true;
                dialog.accept();
                return;
            }
        }

        QMessageBox::warning(&dialog, "Login Failed", "Invalid username or password.");
    });

    // Allow Enter key to trigger login
    connect(usernameInput, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);
    connect(passwordInput, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);

    dialog.exec();
    return accepted;
}

// ==================== TAB CREATION ====================

QWidget* MainWindow::createDepartmentsTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    // Controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    QPushButton *sortBtn = new QPushButton("⬆ Sort by Name (Merge Sort)");
    sortBtn->setObjectName("actionBtn");
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortDepartments);
    controlsLayout->addWidget(sortBtn);

    controlsLayout->addStretch();

    deptSearchInput = new QLineEdit();
    deptSearchInput->setPlaceholderText("Search department by name...");
    deptSearchInput->setFixedWidth(250);
    controlsLayout->addWidget(deptSearchInput);

    QPushButton *searchBtn = new QPushButton("🔍 Search");
    searchBtn->setObjectName("searchBtn");
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchDepartment);
    connect(deptSearchInput, &QLineEdit::returnPressed, this, &MainWindow::onSearchDepartment);
    controlsLayout->addWidget(searchBtn);

    layout->addLayout(controlsLayout);

    // Table
    deptTable = new QTableWidget();
    deptTable->setColumnCount(2);
    deptTable->setHorizontalHeaderLabels({"Code", "Name"});
    deptTable->horizontalHeader()->setStretchLastSection(true);
    deptTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    deptTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    deptTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    deptTable->setAlternatingRowColors(true);
    deptTable->verticalHeader()->setVisible(false);
    layout->addWidget(deptTable);
    return tab;
}

QWidget* MainWindow::createUsersTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    // Controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    
    if (currentRole == "Admin") {
        QPushButton *addUserBtn = new QPushButton("➕ Add User");
        addUserBtn->setObjectName("actionBtn");
        addUserBtn->setStyleSheet("background-color: #0984e3;");
        connect(addUserBtn, &QPushButton::clicked, this, &MainWindow::onAddUser);
        controlsLayout->addWidget(addUserBtn);
    }
    
    controlsLayout->addStretch();
    layout->addLayout(controlsLayout);

    // Table
    usersTable = new QTableWidget();
    usersTable->setColumnCount(3);
    usersTable->setHorizontalHeaderLabels({"Username", "Password", "Role"});
    usersTable->horizontalHeader()->setStretchLastSection(true);
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setAlternatingRowColors(true);
    usersTable->verticalHeader()->setVisible(false);
    layout->addWidget(usersTable);

    return tab;
}

QWidget* MainWindow::createStudentsTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    // Controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    QPushButton *sortBtn = new QPushButton("⬆ Sort by GPA");
    sortBtn->setObjectName("actionBtn");
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortStudents);
    controlsLayout->addWidget(sortBtn);

    QPushButton *sortByIdBtn = new QPushButton("⬆ Sort by ID");
    sortByIdBtn->setObjectName("actionBtn");
    connect(sortByIdBtn, &QPushButton::clicked, this, &MainWindow::onSortStudentsById);
    controlsLayout->addWidget(sortByIdBtn);

    QPushButton *addUserBtn = new QPushButton("➕ Add Student");
    addUserBtn->setObjectName("actionBtn");
    addUserBtn->setStyleSheet("background-color: #0984e3;");
    connect(addUserBtn, &QPushButton::clicked, this, &MainWindow::onAddUser);
    if (currentRole == "Instructor") {
        controlsLayout->addWidget(addUserBtn);
    }

    controlsLayout->addStretch();

    studentSearchInput = new QLineEdit();
    studentSearchInput->setPlaceholderText("Search student by ID...");
    studentSearchInput->setFixedWidth(200);
    controlsLayout->addWidget(studentSearchInput);

    QPushButton *searchBtn = new QPushButton("🔍 Search");
    searchBtn->setObjectName("searchBtn");
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchStudent);
    connect(studentSearchInput, &QLineEdit::returnPressed, this, &MainWindow::onSearchStudent);
    controlsLayout->addWidget(searchBtn);

    layout->addLayout(controlsLayout);

    // Table
    studentTable = new QTableWidget();
    studentTable->setColumnCount(6);
    studentTable->setHorizontalHeaderLabels({"ID", "Name", "GPA", "Year", "Dept", "Phone"});
    studentTable->horizontalHeader()->setStretchLastSection(true);
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setAlternatingRowColors(true);
    studentTable->verticalHeader()->setVisible(false);
    layout->addWidget(studentTable);

    return tab;
}

QWidget* MainWindow::createCoursesTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    // Controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    QPushButton *sortBtn = new QPushButton("⬆ Sort by Capacity");
    sortBtn->setObjectName("actionBtn");
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortCourses);
    controlsLayout->addWidget(sortBtn);

    QPushButton *sortEnrollBtn = new QPushButton("⬆ Sort by Enrolled Count");
    sortEnrollBtn->setObjectName("actionBtn");
    connect(sortEnrollBtn, &QPushButton::clicked, this, &MainWindow::onSortCoursesByEnrollment);
    controlsLayout->addWidget(sortEnrollBtn);

    if (currentRole == "Admin" || currentRole == "Instructor") {
        QPushButton *viewEnrolledBtn = new QPushButton("👁 View Enrolled Students");
        viewEnrolledBtn->setObjectName("actionBtn");
        connect(viewEnrolledBtn, &QPushButton::clicked, this, &MainWindow::onViewEnrolledStudents);
        controlsLayout->addWidget(viewEnrolledBtn);
    }

    QPushButton *enrollBtn = new QPushButton("➕ Enroll in Course");
    enrollBtn->setObjectName("actionBtn");
    enrollBtn->setStyleSheet("background-color: #0984e3;");
    connect(enrollBtn, &QPushButton::clicked, this, &MainWindow::onEnrollStudent);
    controlsLayout->addWidget(enrollBtn);

    controlsLayout->addStretch();
    layout->addLayout(controlsLayout);

    // Table
    courseTable = new QTableWidget();
    courseTable->setColumnCount(5);
    courseTable->setHorizontalHeaderLabels({"Code", "Name", "Credits", "Capacity", "Enrolled"});
    courseTable->horizontalHeader()->setStretchLastSection(true);
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    courseTable->setAlternatingRowColors(true);
    courseTable->verticalHeader()->setVisible(false);
    layout->addWidget(courseTable);

    return tab;
}

QWidget* MainWindow::createMyCoursesTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    int sIdx = system.binarySearchStudent(loggedInStudentId);
    float gpa = (sIdx != -1) ? system.getStudents()[sIdx].getGpa() : 0.0f;
    
    myCreditsLabel = new QLabel("Total Credits: 0 / " + QString::number(system.getMaxCreditHours(gpa)));
    myCreditsLabel->setObjectName("roleLabel");
    myCreditsLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #a29bfe;");
    headerLayout->addWidget(myCreditsLabel);
    headerLayout->addStretch();
    
    QPushButton *dropBtn = new QPushButton("➖ Drop Selected Course");
    dropBtn->setObjectName("warningBtn");
    connect(dropBtn, &QPushButton::clicked, this, &MainWindow::onDropCourse);
    headerLayout->addWidget(dropBtn);
    
    layout->addLayout(headerLayout);

    myCoursesTable = new QTableWidget();
    myCoursesTable->setColumnCount(4);
    myCoursesTable->setHorizontalHeaderLabels({"Code", "Name", "Credits", "Capacity"});
    myCoursesTable->horizontalHeader()->setStretchLastSection(true);
    myCoursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    myCoursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    myCoursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    myCoursesTable->setAlternatingRowColors(true);
    myCoursesTable->verticalHeader()->setVisible(false);
    layout->addWidget(myCoursesTable);

    return tab;
}

QWidget* MainWindow::createAcademicHistoryTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    academicHistoryTable = new QTableWidget();
    academicHistoryTable->setColumnCount(2);
    academicHistoryTable->setHorizontalHeaderLabels({"Course Code", "Grade"});
    academicHistoryTable->horizontalHeader()->setStretchLastSection(true);
    academicHistoryTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    academicHistoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    academicHistoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    academicHistoryTable->setAlternatingRowColors(true);
    academicHistoryTable->verticalHeader()->setVisible(false);
    layout->addWidget(academicHistoryTable);

    return tab;
}

QWidget* MainWindow::createPrerequisitesTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(12);

    // Controls row
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    prereqCourseInput = new QLineEdit();
    prereqCourseInput->setPlaceholderText("Enter course code (e.g., CS401)...");
    prereqCourseInput->setFixedWidth(280);
    controlsLayout->addWidget(prereqCourseInput);

    QPushButton *findBtn = new QPushButton("🔗 View Course Prerequisites");
    findBtn->setObjectName("actionBtn");
    connect(findBtn, &QPushButton::clicked, this, &MainWindow::onFindPrerequisites);
    connect(prereqCourseInput, &QLineEdit::returnPressed, this, &MainWindow::onFindPrerequisites);
    controlsLayout->addWidget(findBtn);

    controlsLayout->addStretch();

    QPushButton *cycleBtn = new QPushButton("🔄 Validate Course Map");
    cycleBtn->setObjectName("warningBtn");
    connect(cycleBtn, &QPushButton::clicked, this, &MainWindow::onCheckCycles);
    controlsLayout->addWidget(cycleBtn);

    layout->addLayout(controlsLayout);

    // Results area
    prereqResultText = new QTextEdit();
    prereqResultText->setReadOnly(true);
    prereqResultText->setObjectName("prereqResults");
    prereqResultText->setPlaceholderText("Prerequisite results will appear here...");
    layout->addWidget(prereqResultText);

    return tab;
}

// ==================== TABLE REFRESH ====================

void MainWindow::refreshUsersTable() {
    if (!usersTable) return;
    
    auto users = system.getAllUsers();
    usersTable->setRowCount(users.size());

    int row = 0;
    for (const auto& u : users) {
        usersTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(u.username)));
        usersTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(u.password)));
        usersTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(u.role)));
        row++;
    }
}

void MainWindow::refreshDepartmentsTable() {
    const auto& depts = system.getDepartments();
    deptTable->setRowCount(depts.size());

    for (int i = 0; i < (int)depts.size(); i++) {
        deptTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(depts[i].code)));
        deptTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(depts[i].name)));
    }
}

void MainWindow::refreshStudentsTable() {
    const auto& studs = system.getStudents();
    studentTable->setRowCount(studs.size());

    for (int i = 0; i < (int)studs.size(); i++) {
        studentTable->setItem(i, 0, new QTableWidgetItem(QString::number(studs[i].id)));
        studentTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(studs[i].name)));
        studentTable->setItem(i, 2, new QTableWidgetItem(QString::number(studs[i].gpa, 'f', 2)));
        studentTable->setItem(i, 3, new QTableWidgetItem(QString::number(studs[i].year)));
        studentTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(studs[i].getDepartmentCode())));
        studentTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(studs[i].phone)));
    }
}

void MainWindow::refreshCoursesTable() {
    const auto& crses = system.getCourses();
    courseTable->setRowCount(crses.size());

    for (int i = 0; i < (int)crses.size(); i++) {
        courseTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(crses[i].code)));
        courseTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(crses[i].name)));
        courseTable->setItem(i, 2, new QTableWidgetItem(QString::number(crses[i].getCreditHours())));
        courseTable->setItem(i, 3, new QTableWidgetItem(QString::number(crses[i].capacity)));
        
        QString enrolledText = QString::number(crses[i].getEnrolledCount()) + " / " + QString::number(crses[i].capacity);
        courseTable->setItem(i, 4, new QTableWidgetItem(enrolledText));
    }
}

void MainWindow::refreshMyCoursesTable() {
    if (!myCoursesTable) return;
    const auto& courses = system.getCourses();
    
    int count = 0;
    int totalCredits = 0;
    for (const auto& c : courses) {
        if (c.isStudentEnrolled(loggedInStudentId)) {
            count++;
            totalCredits += c.getCreditHours();
        }
    }
    
    myCoursesTable->setRowCount(count);
    int row = 0;
    for (const auto& c : courses) {
        if (c.isStudentEnrolled(loggedInStudentId)) {
            myCoursesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c.getCode())));
            myCoursesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c.getName())));
            myCoursesTable->setItem(row, 2, new QTableWidgetItem(QString::number(c.getCreditHours())));
            myCoursesTable->setItem(row, 3, new QTableWidgetItem(QString::number(c.getCapacity())));
            row++;
        }
    }

    int sIdx = system.binarySearchStudent(loggedInStudentId);
    float gpa = (sIdx != -1) ? system.getStudents()[sIdx].getGpa() : 0.0f;
    myCreditsLabel->setText("Total Credits: " + QString::number(totalCredits) + " / " + QString::number(system.getMaxCreditHours(gpa)));
}

void MainWindow::refreshAcademicHistoryTable() {
    if (!academicHistoryTable) return;
    
    int sIdx = system.binarySearchStudent(loggedInStudentId);
    if (sIdx == -1) return;
    
    const auto& history = system.getStudents()[sIdx].getCourseHistory();
    academicHistoryTable->setRowCount(history.size());
    
    for (int i = 0; i < (int)history.size(); i++) {
        academicHistoryTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(history[i].courseCode)));
        academicHistoryTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(history[i].grade)));
    }
}

// ==================== SLOTS ====================

void MainWindow::onSortCoursesByEnrollment() {
    system.sortCoursesByEnrolledCount();
    refreshCoursesTable();
}

void MainWindow::onSortDepartments() {
    system.sortDepartmentsByName();
    refreshDepartmentsTable();
}

void MainWindow::onSearchDepartment() {
    QString searchName = deptSearchInput->text().trimmed();
    if (searchName.isEmpty()) {
        QMessageBox::information(this, "Search", "Please enter a department name to search.");
        return;
    }

    int index = system.linearSearchDepartment(searchName.toStdString());
    refreshDepartmentsTable(); 

    if (index >= 0) {
        deptTable->selectRow(index);
        deptTable->scrollToItem(deptTable->item(index, 0));
        QMessageBox::information(this, "Search Result", "Department '" + searchName + "' found successfully!");
    } else {
        deptTable->clearSelection();
        QMessageBox::information(this, "Search Result",
            "❌ Department '" + searchName + "' not found.");
    }
}

void MainWindow::onSortStudents() {
    system.sortStudentsByGPA();
    refreshStudentsTable();
}

void MainWindow::onSortStudentsById() {
    system.sortStudentsById();
    refreshStudentsTable();
}

void MainWindow::onSearchStudent() {
    QString searchId = studentSearchInput->text().trimmed();
    if (searchId.isEmpty()) {
        QMessageBox::information(this, "Search", "Please enter a student ID to search.");
        return;
    }

    bool ok;
    int id = searchId.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid numeric ID.");
        return;
    }

    int index = system.binarySearchStudent(id);
    refreshStudentsTable();

    if (index >= 0) {
        studentTable->selectRow(index);
        studentTable->scrollToItem(studentTable->item(index, 0));
        QMessageBox::information(this, "Search Result", "Student with ID " + QString::number(id) + " found successfully!");
    } else {
        studentTable->clearSelection();
        QMessageBox::information(this, "Search Result",
            "❌ Student with ID " + QString::number(id) + " not found.");
    }
}

void MainWindow::onAddUser() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add New User");
    dialog.setFixedSize(380, 450);

    QVBoxLayout layout(&dialog);
    QFormLayout form(&dialog);

    QComboBox *roleCombo = new QComboBox();
    
    // Only Admin can add other Admins/Instructors. Instructors can only add Students.
    if (currentRole == "Admin") {
        roleCombo->addItems({"Student", "Instructor", "Admin"});
    } else {
        roleCombo->addItems({"Student"});
    }

    QLineEdit *userInput = new QLineEdit();
    QLineEdit *passInput = new QLineEdit();
    QSpinBox *idInput = new QSpinBox();
    idInput->setRange(1, 99999);
    QLineEdit *nameInput = new QLineEdit();
    QDoubleSpinBox *gpaInput = new QDoubleSpinBox();
    gpaInput->setRange(0.0, 4.0);
    gpaInput->setSingleStep(0.1);
    QSpinBox *yearInput = new QSpinBox();
    yearInput->setRange(1, 5);
    
    QComboBox *deptCombo = new QComboBox();
    for (const auto& d : system.getDepartments()) {
        deptCombo->addItem(QString::fromStdString(d.code));
    }
    
    QLineEdit *phoneInput = new QLineEdit();

    form.addRow("Role:", roleCombo);
    form.addRow("Username:", userInput);
    form.addRow("Password:", passInput);
    
    // Dynamic fields wrapper so we can show/hide them easily
    QWidget *dynamicWidget = new QWidget();
    QFormLayout *dynamicForm = new QFormLayout(dynamicWidget);
    dynamicForm->setContentsMargins(0, 0, 0, 0);
    
    // Label/Field pairs for dynamic visibility
    QLabel *idLabel = new QLabel("ID:");
    dynamicForm->addRow(idLabel, idInput);
    
    QLabel *nameLabel = new QLabel("Full Name:");
    dynamicForm->addRow(nameLabel, nameInput);
    
    QLabel *gpaLabel = new QLabel("GPA:");
    dynamicForm->addRow(gpaLabel, gpaInput);
    
    QLabel *yearLabel = new QLabel("Year:");
    dynamicForm->addRow(yearLabel, yearInput);
    
    QLabel *deptLabel = new QLabel("Department:");
    dynamicForm->addRow(deptLabel, deptCombo);
    
    QLabel *phoneLabel = new QLabel("Phone:");
    dynamicForm->addRow(phoneLabel, phoneInput);
    
    layout.addLayout(&form);
    layout.addWidget(dynamicWidget);

    auto updateFields = [&]() {
        QString role = roleCombo->currentText();
        
        // Hide everything first
        idLabel->hide(); idInput->hide();
        nameLabel->hide(); nameInput->hide();
        gpaLabel->hide(); gpaInput->hide();
        yearLabel->hide(); yearInput->hide();
        deptLabel->hide(); deptCombo->hide();
        phoneLabel->hide(); phoneInput->hide();

        if (role == "Student") {
            idLabel->show(); idInput->show();
            nameLabel->show(); nameInput->show();
            gpaLabel->show(); gpaInput->show();
            yearLabel->show(); yearInput->show();
            deptLabel->show(); deptCombo->show();
            phoneLabel->show(); phoneInput->show();
        } else if (role == "Instructor") {
            idLabel->show(); idInput->show();
            nameLabel->show(); nameInput->show();
            deptLabel->show(); deptCombo->show();
        }
    };
    
    connect(roleCombo, &QComboBox::currentTextChanged, updateFields);
    updateFields(); // Initial call

    QDialogButtonBox btns(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&btns, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&btns, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout.addWidget(&btns);

    if (dialog.exec() == QDialog::Accepted) {
        if (userInput->text().isEmpty() || passInput->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Username and Password are required.");
            return;
        }
        
        QString role = roleCombo->currentText();
        
        if (role == "Student") {
            system.addStudent(userInput->text().toStdString(),
                              passInput->text().toStdString(),
                              idInput->value(),
                              nameInput->text().toStdString(),
                              gpaInput->value(),
                              yearInput->value(),
                              phoneInput->text().toStdString(),
                              deptCombo->currentText().toStdString());
            refreshStudentsTable();
        } else if (role == "Instructor") {
            system.addInstructor(userInput->text().toStdString(),
                                 passInput->text().toStdString(),
                                 idInput->value(),
                                 nameInput->text().toStdString(),
                                 deptCombo->currentText().toStdString());
        } else if (role == "Admin") {
            system.addAdmin(userInput->text().toStdString(),
                            passInput->text().toStdString());
        }
        
        refreshUsersTable();
        QMessageBox::information(this, "Success", "User added successfully!");
    }
}

void MainWindow::onSortCourses() {
    system.sortCoursesByCapacity();
    refreshCoursesTable();
}

void MainWindow::onEnrollStudent() {
    QDialog dialog(this);
    dialog.setWindowTitle("Enroll Student in Course");
    dialog.setFixedSize(300, 200);

    QVBoxLayout layout(&dialog);
    QFormLayout form(&dialog);

    QComboBox *courseCombo = new QComboBox();
    for (const auto& c : system.getCourses()) {
        courseCombo->addItem(QString::fromStdString(c.getCode()));
    }

    QSpinBox *studentIdInput = new QSpinBox();
    studentIdInput->setRange(1, 99999);
    
    // If logged in as student, pre-fill and disable the ID input
    if (currentRole == "Student" && loggedInStudentId != -1) {
        studentIdInput->setValue(loggedInStudentId);
        studentIdInput->setEnabled(false);
    }

    form.addRow("Course:", courseCombo);
    form.addRow("Student ID:", studentIdInput);
    layout.addLayout(&form);

    QDialogButtonBox btns(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&btns, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&btns, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout.addWidget(&btns);

    if (dialog.exec() == QDialog::Accepted) {
        string courseCode = courseCombo->currentText().toStdString();
        int studentId = studentIdInput->value();

        string result = system.enrollStudentInCourse(studentId, courseCode);
        if (result == "OK") {
            refreshCoursesTable();
            if (currentRole == "Student") {
                refreshMyCoursesTable();
            }
            QMessageBox::information(this, "Success", "Student enrolled successfully!");
        } else {
            QMessageBox::warning(this, "Enrollment Failed", QString::fromStdString(result));
        }
    }
}

void MainWindow::onViewEnrolledStudents() {
    QList<QTableWidgetItem*> selectedItems = courseTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Select Course", "Please select a course to view its students.");
        return;
    }
    
    int row = selectedItems.first()->row();
    QString courseCode = courseTable->item(row, 0)->text();
    
    const Course* selectedCourse = nullptr;
    for (const auto& c : system.getCourses()) {
        if (c.getCode() == courseCode.toStdString()) {
            selectedCourse = &c;
            break;
        }
    }
    
    if (!selectedCourse) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Enrolled Students - " + courseCode);
    dialog.setFixedSize(450, 350);
    QVBoxLayout layout(&dialog);
    
    QLabel *headerLabel = new QLabel("Students in " + QString::fromStdString(selectedCourse->getName()));
    headerLabel->setObjectName("roleLabel");
    headerLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 10px; color: #a29bfe;");
    layout.addWidget(headerLabel);
    
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"ID", "Name", "Department"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    
    const auto& enrolledIds = selectedCourse->getEnrolledStudents();
    table->setRowCount(enrolledIds.size());
    
    int tableRow = 0;
    for (int studentId : enrolledIds) {
        int idx = system.binarySearchStudent(studentId);
        if (idx != -1) {
            const auto& s = system.getStudents()[idx];
            table->setItem(tableRow, 0, new QTableWidgetItem(QString::number(s.getId())));
            table->setItem(tableRow, 1, new QTableWidgetItem(QString::fromStdString(s.getName())));
            table->setItem(tableRow, 2, new QTableWidgetItem(QString::fromStdString(s.getDepartmentCode())));
            tableRow++;
        }
    }
    
    layout.addWidget(table);
    QDialogButtonBox btns(QDialogButtonBox::Ok);
    connect(&btns, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    layout.addWidget(&btns);
    
    dialog.exec();
}

void MainWindow::onDropCourse() {
    QList<QTableWidgetItem*> selectedItems = myCoursesTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Select Course", "Please select a course to drop.");
        return;
    }
    
    int row = selectedItems.first()->row();
    QString courseCode = myCoursesTable->item(row, 0)->text();
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Drop Course", "Are you sure you want to drop " + courseCode + "?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        string result = system.dropStudentFromCourse(loggedInStudentId, courseCode.toStdString());
        if (result == "OK") {
            refreshCoursesTable();
            refreshMyCoursesTable();
            QMessageBox::information(this, "Success", "Course dropped successfully!");
        } else {
            QMessageBox::warning(this, "Drop Failed", QString::fromStdString(result));
        }
    }
}

void MainWindow::onFindPrerequisites() {
    QString courseCode = prereqCourseInput->text().trimmed().toUpper();
    if (courseCode.isEmpty()) {
        QMessageBox::information(this, "Prerequisites", "Please enter a course code.");
        return;
    }

    vector<string> prereqs = system.getPrerequisitesBFS(courseCode.toStdString());
    
    QString result = "<div style='font-size: 14px;'>";
    result += "<h3>Prerequisites for " + courseCode + "</h3>";

    if (prereqs.empty()) {
        result += "<p style='color: #888;'>No prerequisites found for this course.</p>";
    } else {
        result += "<ul>";
        for (const auto& p : prereqs) {
            result += "<li><b>" + QString::fromStdString(p) + "</b></li>";
        }
        result += "</ul>";
        result += "<p><i>Total: " + QString::number(prereqs.size()) + " prerequisite(s)</i></p>";
    }

    prereqResultText->setHtml(result);
}

void MainWindow::onCheckCycles() {
    bool hasCycle = system.checkForCycles();

    QString result;
    if (hasCycle) {
        result = "<h3 style='color: #e74c3c;'>⚠️ Cycle Detected!</h3>"
                 "<p>A circular dependency was found in the prerequisite graph. "
                 "This means some courses have impossible prerequisite chains.</p>";
    } else {
        result = "<h3 style='color: #2ecc71;'>✅ No Cycles Detected</h3>"
                 "<p>The prerequisite graph is valid. All prerequisite chains are acyclic.</p>";
    }

    prereqResultText->setHtml(result);
}

void MainWindow::onLogout() {
    // Clear the central widget
    QWidget *centralWidget = this->centralWidget();
    if (centralWidget) {
        centralWidget->deleteLater();
    }
    
    // Reset state
    currentRole = "";
    loggedInStudentId = -1;
    
    // Show login dialog again
    if (!showLoginDialog()) {
        QTimer::singleShot(0, qApp, &QApplication::quit);
        return;
    }
    
    // Rebuild UI for the new role
    setupMainUI();
}

// ==================== STYLESHEET ====================

void MainWindow::applyStyleSheet() {
    qApp->setStyleSheet(R"(
        /* ---- Global ---- */
        QMainWindow, QDialog {
            background-color: #1a1b2e;
            color: #e0e0e0;
        }

        QWidget {
            font-family: 'Segoe UI', 'Inter', 'Roboto', sans-serif;
            font-size: 13px;
            color: #e0e0e0;
        }

        /* ---- Header ---- */
        #headerLabel {
            font-size: 26px;
            font-weight: bold;
            color: #ffffff;
            padding: 8px 0;
            border-bottom: 2px solid #6c5ce7;
            margin-bottom: 4px;
        }

        #roleLabel {
            font-size: 13px;
            color: #a29bfe;
            padding-bottom: 6px;
        }

        /* ---- Login Dialog ---- */
        #loginDialog {
            background-color: #1a1b2e;
        }

        #loginTitle {
            font-size: 24px;
            font-weight: bold;
            color: #ffffff;
            margin-bottom: 10px;
        }

        #loginInput {
            background-color: #2d2e4a;
            border: 1px solid #4a4b6a;
            border-radius: 6px;
            padding: 8px 12px;
            color: #ffffff;
            font-size: 14px;
        }

        #loginInput:focus {
            border-color: #6c5ce7;
        }

        #hintLabel {
            color: #888;
            font-size: 11px;
            font-style: italic;
        }

        #loginBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #6c5ce7, stop:1 #a29bfe);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 15px;
            font-weight: bold;
            padding: 8px;
        }

        #loginBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #7d6ff0, stop:1 #b3acfe);
        }

        #loginBtn:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #5b4bd5, stop:1 #918af0);
        }

        /* ---- Tab Widget ---- */
        QTabWidget::pane {
            border: 1px solid #3d3e5c;
            border-radius: 8px;
            background-color: #22233a;
            top: -1px;
        }

        QTabBar::tab {
            background-color: #2d2e4a;
            color: #a0a0c0;
            padding: 10px 20px;
            margin-right: 2px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            font-size: 13px;
            font-weight: 500;
        }

        QTabBar::tab:selected {
            background-color: #22233a;
            color: #ffffff;
            border-bottom: 3px solid #6c5ce7;
        }

        QTabBar::tab:hover:!selected {
            background-color: #353660;
            color: #d0d0e0;
        }

        /* ---- Tables ---- */
        QTableWidget {
            background-color: #22233a;
            alternate-background-color: #282948;
            gridline-color: #3d3e5c;
            border: 1px solid #3d3e5c;
            border-radius: 6px;
            selection-background-color: #6c5ce7;
            selection-color: #ffffff;
            color: #ffffff;
        }

        QTableWidget::item {
            padding: 6px 10px;
            color: #ffffff;
        }

        QHeaderView::section {
            background-color: #2d2e4a;
            color: #a29bfe;
            padding: 8px 10px;
            border: none;
            border-bottom: 2px solid #6c5ce7;
            font-weight: bold;
            font-size: 12px;
            text-transform: uppercase;
        }

        /* ---- Buttons ---- */
        #actionBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #6c5ce7, stop:1 #a29bfe);
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 18px;
            font-weight: bold;
            font-size: 13px;
        }

        #actionBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #7d6ff0, stop:1 #b3acfe);
        }

        #actionBtn:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #5b4bd5, stop:1 #918af0);
        }

        #searchBtn {
            background-color: #00b894;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
        }

        #searchBtn:hover {
            background-color: #00d1a0;
        }

        #searchBtn:pressed {
            background-color: #009b7d;
        }

        #warningBtn {
            background-color: #e17055;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
        }

        #warningBtn:hover {
            background-color: #f0806a;
        }

        #warningBtn:pressed {
            background-color: #c0604a;
        }

        /* ---- Input Fields ---- */
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background-color: #2d2e4a;
            border: 1px solid #4a4b6a;
            border-radius: 6px;
            padding: 8px 12px;
            color: #ffffff;
            font-size: 13px;
        }
        
        QComboBox QAbstractItemView {
            background-color: #2d2e4a;
            color: #ffffff;
            selection-background-color: #6c5ce7;
        }

        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #6c5ce7;
        }

        /* ---- Text Edit ---- */
        #prereqResults {
            background-color: #22233a;
            border: 1px solid #3d3e5c;
            border-radius: 6px;
            padding: 12px;
            color: #e0e0e0;
            font-size: 14px;
        }

        /* ---- Scrollbars ---- */
        QScrollBar:vertical {
            background-color: #1a1b2e;
            width: 10px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background-color: #4a4b6a;
            border-radius: 5px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #6c5ce7;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            background-color: #1a1b2e;
            height: 10px;
            border-radius: 5px;
        }

        QScrollBar::handle:horizontal {
            background-color: #4a4b6a;
            border-radius: 5px;
            min-width: 30px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: #6c5ce7;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        /* ---- Message Boxes ---- */
        QMessageBox {
            background-color: #1a1b2e;
        }

        QMessageBox QLabel {
            color: #e0e0e0;
            font-size: 13px;
        }

        QMessageBox QPushButton {
            background-color: #6c5ce7;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 20px;
            font-weight: bold;
        }

        QMessageBox QPushButton:hover {
            background-color: #7d6ff0;
        }

        /* ---- Form Labels ---- */
        QLabel {
            color: #c0c0d0;
        }
    )");
}
