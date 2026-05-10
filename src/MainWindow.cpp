#include "../include/MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QStackedWidget>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize data
    system.initializeData();

    // Apply stylesheet
    applyStyleSheet();

    // Show login dialog
    if (!showLoginDialog()) {
        QTimer::singleShot(0, qApp, &QApplication::quit);
        return;
    }

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

    // Header
    QLabel *headerLabel = new QLabel("🎓 College Management System");
    headerLabel->setObjectName("headerLabel");
    mainLayout->addWidget(headerLabel);

    // Role indicator
    QLabel *roleLabel = new QLabel("Logged in as: " + currentRole);
    roleLabel->setObjectName("roleLabel");
    mainLayout->addWidget(roleLabel);

    // Tab widget
    tabWidget = new QTabWidget();
    tabWidget->setObjectName("mainTabs");

    tabWidget->addTab(createDepartmentsTab(), "📁 Departments");
    tabWidget->addTab(createStudentsTab(), "🎓 Students");
    tabWidget->addTab(createCoursesTab(), "📚 Courses");
    tabWidget->addTab(createPrerequisitesTab(), "🔗 Prerequisites");

    mainLayout->addWidget(tabWidget);

    // Populate tables
    refreshDepartmentsTable();
    refreshStudentsTable();
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
        const auto& students = system.getStudents();
        for (const auto& s : students) {
            if (QString::fromStdString(s.getUsername()) == user &&
                s.checkPassword(pass.toStdString())) {
                currentRole = "Student";
                accepted = true;
                dialog.accept();
                return;
            }
        }

        // Admin/Instructor hardcoded login
        if (user == "admin" && pass == "admin") {
            currentRole = "Instructor";
            accepted = true;
            dialog.accept();
            return;
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

QWidget* MainWindow::createStudentsTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(10);

    // Controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    QPushButton *sortBtn = new QPushButton("⬆ Sort by GPA (Selection Sort)");
    sortBtn->setObjectName("actionBtn");
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortStudents);
    controlsLayout->addWidget(sortBtn);

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
    studentTable->setColumnCount(5);
    studentTable->setHorizontalHeaderLabels({"ID", "Name", "GPA", "Year", "Phone"});
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

    QPushButton *sortBtn = new QPushButton("⬆ Sort by Capacity (Bubble Sort)");
    sortBtn->setObjectName("actionBtn");
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortCourses);
    controlsLayout->addWidget(sortBtn);

    controlsLayout->addStretch();
    layout->addLayout(controlsLayout);

    // Table
    courseTable = new QTableWidget();
    courseTable->setColumnCount(3);
    courseTable->setHorizontalHeaderLabels({"Code", "Name", "Capacity"});
    courseTable->horizontalHeader()->setStretchLastSection(true);
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    courseTable->setAlternatingRowColors(true);
    courseTable->verticalHeader()->setVisible(false);
    layout->addWidget(courseTable);

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

    QPushButton *findBtn = new QPushButton("🔗 Find Prerequisites (BFS)");
    findBtn->setObjectName("actionBtn");
    connect(findBtn, &QPushButton::clicked, this, &MainWindow::onFindPrerequisites);
    connect(prereqCourseInput, &QLineEdit::returnPressed, this, &MainWindow::onFindPrerequisites);
    controlsLayout->addWidget(findBtn);

    controlsLayout->addStretch();

    QPushButton *cycleBtn = new QPushButton("🔄 Check for Cycles (DFS)");
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
        studentTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(studs[i].phone)));
    }
}

void MainWindow::refreshCoursesTable() {
    const auto& crses = system.getCourses();
    courseTable->setRowCount(crses.size());

    for (int i = 0; i < (int)crses.size(); i++) {
        courseTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(crses[i].code)));
        courseTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(crses[i].name)));
        courseTable->setItem(i, 2, new QTableWidgetItem(QString::number(crses[i].capacity)));
    }
}

// ==================== SLOTS ====================

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

    int index = system.binarySearchDepartment(searchName.toStdString());
    refreshDepartmentsTable(); // Refresh because binary search sorts internally

    if (index >= 0) {
        deptTable->selectRow(index);
        deptTable->scrollToItem(deptTable->item(index, 0));
        QMessageBox::information(this, "Search Result",
            "✅ Department '" + searchName + "' found at index " + QString::number(index) + ".");
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
    refreshStudentsTable(); // Refresh because binary search sorts internally

    if (index >= 0) {
        studentTable->selectRow(index);
        studentTable->scrollToItem(studentTable->item(index, 0));
        QMessageBox::information(this, "Search Result",
            "✅ Student with ID " + QString::number(id) + " found at index " + QString::number(index) + ".");
    } else {
        studentTable->clearSelection();
        QMessageBox::information(this, "Search Result",
            "❌ Student with ID " + QString::number(id) + " not found.");
    }
}

void MainWindow::onSortCourses() {
    system.sortCoursesByCapacity();
    refreshCoursesTable();
}

void MainWindow::onFindPrerequisites() {
    QString courseCode = prereqCourseInput->text().trimmed().toUpper();
    if (courseCode.isEmpty()) {
        QMessageBox::information(this, "Prerequisites", "Please enter a course code.");
        return;
    }

    vector<string> prereqs = system.getPrerequisitesBFS(courseCode.toStdString());

    QString result;
    result += "<h3>Prerequisites for " + courseCode + " (BFS Traversal)</h3>";

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
        }

        QTableWidget::item {
            padding: 6px 10px;
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
        QLineEdit {
            background-color: #2d2e4a;
            border: 1px solid #4a4b6a;
            border-radius: 6px;
            padding: 8px 12px;
            color: #ffffff;
            font-size: 13px;
        }

        QLineEdit:focus {
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
