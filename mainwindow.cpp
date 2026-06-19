#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QStackedWidget>
#include <QPainter>
#include <QPixmap>

class BackgroundWidget : public QWidget {
public:
    BackgroundWidget(const QString& imagePath, QWidget* parent = nullptr)
        : QWidget(parent) {
        pixmap.load(imagePath);
    }
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        if (!pixmap.isNull()) {
            QPixmap scaled = pixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            int x = (scaled.width()  - width())  / 2;
            int y = (scaled.height() - height()) / 2;
            painter.drawPixmap(-x, -y, scaled);
        } else {
            painter.fillRect(rect(), QColor("#0a0a0a"));
        }
        QWidget::paintEvent(event);
    }
private:
    QPixmap pixmap;
};

static bool isNumericId(const QString& str) {
    if (str.isEmpty()) return false;
    for (const QChar &ch : str) {
        if (!ch.isDigit()) return false;
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    university = new University("ENSIA University");
    university->loadData("udrms_data.txt");

    if (university->getDormitories().empty()) {
        university->addDormitory("Dormitory A", 50);
        university->addDormitory("Dormitory B", 30);
        university->findDormitory("Dormitory A")->addRoom(101, 3);
        university->findDormitory("Dormitory A")->addRoom(102, 2);
        university->findDormitory("Dormitory B")->addRoom(201, 4);
    }

    setupUI();
    showStudentPanel();
}

MainWindow::~MainWindow() {
    university->saveData("udrms_data.txt");
    delete ui;
    delete university;
}

void MainWindow::setupUI() {
    setWindowTitle("UDRMS — ENSIA University");
    setMinimumSize(1200, 720);

    QFont appFont("Segoe UI", 10);
    qApp->setFont(appFont);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* sidebar = new QWidget();
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet("background-color: #0a0a0a; border-right: 1px solid #c9a84c;");
    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(0, 0, 0, 0);
    sideLayout->setSpacing(0);

    QWidget* logoArea = new QWidget();
    logoArea->setFixedHeight(120);
    logoArea->setStyleSheet("background-color: #0a0a0a; border-bottom: 1px solid #c9a84c;");
    QVBoxLayout* logoLayout = new QVBoxLayout(logoArea);

    QLabel* logo = new QLabel("🎓");
    logo->setStyleSheet("font-size: 36px; border: none;");
    logo->setAlignment(Qt::AlignCenter);

    QLabel* appName = new QLabel("UDRMS");
    appName->setStyleSheet("color: #c9a84c; font-size: 20px; font-weight: bold; letter-spacing: 4px; border: none;");
    appName->setAlignment(Qt::AlignCenter);

    QLabel* subName = new QLabel("ENSIA University");
    subName->setStyleSheet("color: #8a7a5a; font-size: 9px; letter-spacing: 2px; border: none;");
    subName->setAlignment(Qt::AlignCenter);

    logoLayout->addWidget(logo);
    logoLayout->addWidget(appName);
    logoLayout->addWidget(subName);

    QString navBtn =
        "QPushButton { background-color: transparent; color: #d4c5a0; border: none;"
        "border-left: 3px solid transparent; padding: 18px 24px; font-size: 13px;"
        "font-weight: bold; letter-spacing: 1px; text-align: left; }"
        "QPushButton:hover { background-color: #1a1a1a; border-left: 3px solid #c9a84c; color: #c9a84c; }"
        "QPushButton:pressed { background-color: #222222; }";

    QPushButton* btnStudents = new QPushButton("  👤   STUDENTS");
    QPushButton* btnDorms    = new QPushButton("  🏠   DORMITORIES");
    QPushButton* btnRest     = new QPushButton("  🍽   RESTAURANT");
    btnStudents->setStyleSheet(navBtn);
    btnDorms->setStyleSheet(navBtn);
    btnRest->setStyleSheet(navBtn);
    btnStudents->setCursor(Qt::PointingHandCursor);
    btnDorms->setCursor(Qt::PointingHandCursor);
    btnRest->setCursor(Qt::PointingHandCursor);

    QLabel* version = new QLabel("v1.0  •  2025-2026");
    version->setStyleSheet("color: #4a4a4a; font-size: 8px; letter-spacing: 1px; border: none; padding: 10px;");
    version->setAlignment(Qt::AlignCenter);

    sideLayout->addWidget(logoArea);
    sideLayout->addSpacing(20);
    sideLayout->addWidget(btnStudents);
    sideLayout->addWidget(btnDorms);
    sideLayout->addWidget(btnRest);
    sideLayout->addStretch();
    sideLayout->addWidget(version);

    QStackedWidget* stack = new QStackedWidget();
    studentPanel    = new BackgroundWidget(":/University.jpg");
    dormitoryPanel  = new BackgroundWidget(":/dormitory.jpg");
    restaurantPanel = new BackgroundWidget(":/restaurant.jpg");

    setupStudentPanel();
    setupDormitoryPanel();
    setupRestaurantPanel();

    stack->addWidget(studentPanel);
    stack->addWidget(dormitoryPanel);
    stack->addWidget(restaurantPanel);

    connect(btnStudents, &QPushButton::clicked, this, &MainWindow::showStudentPanel);
    connect(btnDorms,    &QPushButton::clicked, this, &MainWindow::showDormitoryPanel);
    connect(btnRest,     &QPushButton::clicked, this, &MainWindow::showRestaurantPanel);

    connect(btnStudents, &QPushButton::clicked, [stack, this]{ stack->setCurrentWidget(studentPanel); });
    connect(btnDorms,    &QPushButton::clicked, [stack, this]{ stack->setCurrentWidget(dormitoryPanel); });
    connect(btnRest,     &QPushButton::clicked, [stack, this]{ stack->setCurrentWidget(restaurantPanel); });

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
}

static QString cardStyle() {
    return "background-color: rgba(10, 10, 10, 0.75); border: 1px solid #c9a84c; border-radius: 12px;";
}

static QString actionBtnStyle() {
    return
        "QPushButton { background-color: transparent; color: #c9a84c; border: 1px solid #c9a84c;"
        "border-radius: 6px; padding: 8px 16px; font-size: 11px; font-weight: bold; letter-spacing: 1px; }"
        "QPushButton:hover { background-color: #c9a84c; color: #0a0a0a; }"
        "QPushButton:pressed { background-color: #a07830; }";
}

static QString tableStyle() {
    return
        "QTableWidget { background-color: rgba(10,10,10,0.6); color: #e8dcc8; border: 1px solid #c9a84c;"
        "border-radius: 8px; gridline-color: #2a2a2a; font-size: 11px; }"
        "QHeaderView::section { background-color: #0a0a0a; color: #c9a84c; padding: 8px; border: none;"
        "font-weight: bold; letter-spacing: 1px; font-size: 11px; }"
        "QTableWidget::item:selected { background-color: rgba(201,168,76,0.3); color: #c9a84c; }";
}

void MainWindow::setupStudentPanel() {
    QVBoxLayout* layout = new QVBoxLayout(studentPanel);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QWidget* titleCard = new QWidget();
    titleCard->setStyleSheet(cardStyle());
    titleCard->setFixedHeight(70);
    QHBoxLayout* titleLayout = new QHBoxLayout(titleCard);
    QLabel* title = new QLabel("STUDENT MANAGEMENT");
    title->setStyleSheet("color: #c9a84c; font-size: 22px; font-weight: bold; letter-spacing: 4px; background: transparent; border: none;");
    QLabel* icon = new QLabel("👤");
    icon->setStyleSheet("font-size: 28px; background: transparent; border: none;");
    titleLayout->addWidget(icon);
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    layout->addWidget(titleCard);

    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* btnAdd    = new QPushButton("＋  ADD STUDENT");
    QPushButton* btnRemove = new QPushButton("－  REMOVE STUDENT");
    btnAdd->setStyleSheet(actionBtnStyle());
    btnRemove->setStyleSheet(actionBtnStyle());
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnRemove->setCursor(Qt::PointingHandCursor);
    btnAdd->setFixedHeight(38);
    btnRemove->setFixedHeight(38);
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnRemove);
    btnRow->addStretch();
    layout->addLayout(btnRow);

    studentTable = new QTableWidget();
    studentTable->setColumnCount(5);
    studentTable->setHorizontalHeaderLabels({"ID", "FULL NAME", "YEAR", "DORMITORY", "ROOM"});
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setStyleSheet(tableStyle());
    studentTable->verticalHeader()->setVisible(false);
    layout->addWidget(studentTable);

    connect(btnAdd,    &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeStudent);
    connect(studentTable, &QTableWidget::cellClicked, this, &MainWindow::onStudentSelected);
}

void MainWindow::setupDormitoryPanel() {
    QVBoxLayout* layout = new QVBoxLayout(dormitoryPanel);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QWidget* titleCard = new QWidget();
    titleCard->setStyleSheet(cardStyle());
    titleCard->setFixedHeight(70);
    QHBoxLayout* titleLayout = new QHBoxLayout(titleCard);
    QLabel* icon = new QLabel("🏠");
    icon->setStyleSheet("font-size: 28px; background: transparent; border: none;");
    QLabel* title = new QLabel("DORMITORY & ROOM WORKSPACE");
    title->setStyleSheet("color: #c9a84c; font-size: 22px; font-weight: bold; letter-spacing: 4px; background: transparent; border: none;");
    titleLayout->addWidget(icon);
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    layout->addWidget(titleCard);

    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* btnAddDorm    = new QPushButton("＋  NEW DORM");
    QPushButton* btnRemoveDorm = new QPushButton("－  REMOVE DORM");
    QPushButton* btnAddRoom    = new QPushButton("＋  NEW ROOM");
    QPushButton* btnAssign     = new QPushButton("✔  CHECK-IN STUDENT");
    QPushButton* btnUnassign   = new QPushButton("✖  KICK FROM ROOM");
    QPushButton* btnTransfer   = new QPushButton("⇆  TRANSFER ROOM");

    for (auto btn : {btnAddDorm, btnRemoveDorm, btnAddRoom, btnAssign, btnUnassign, btnTransfer}) {
        btn->setStyleSheet(actionBtnStyle());
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(38);
        btnRow->addWidget(btn);
    }
    btnRow->addStretch();
    layout->addLayout(btnRow);

    QHBoxLayout* content = new QHBoxLayout();
    content->setSpacing(12);

    // Column 1
    QWidget* leftCard = new QWidget();
    leftCard->setStyleSheet(cardStyle());
    leftCard->setFixedWidth(210);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftCard);
    QLabel* dormLabel = new QLabel("1. SELECT DORMITORY");
    dormLabel->setStyleSheet("color: #c9a84c; font-weight: bold; font-size: 10px; letter-spacing: 1px; background: transparent; border: none;");
    dormList = new QListWidget();
    dormList->setStyleSheet(
        "QListWidget { background: transparent; color: #e8dcc8; border: none; font-size: 11px; }"
        "QListWidget::item { border-bottom: 1px solid #1a1a1a; padding: 6px 2px; }"
        "QListWidget::item:selected { background: rgba(201,168,76,0.2); color: #c9a84c; }"
        );
    leftLayout->addWidget(dormLabel);
    leftLayout->addWidget(dormList);

    // Column 2
    QWidget* centerCard = new QWidget();
    centerCard->setStyleSheet(cardStyle());
    centerCard->setFixedWidth(260);
    QVBoxLayout* centerLayout = new QVBoxLayout(centerCard);
    QLabel* roomLabel = new QLabel("2. SELECT ROOM");
    roomLabel->setStyleSheet("color: #c9a84c; font-weight: bold; font-size: 10px; letter-spacing: 1px; background: transparent; border: none;");
    roomTable = new QTableWidget();
    roomTable->setColumnCount(2);
    roomTable->setHorizontalHeaderLabels({"ROOM", "OCCUPANCY"});
    roomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    roomTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    roomTable->setStyleSheet(tableStyle());
    roomTable->verticalHeader()->setVisible(false);
    centerLayout->addWidget(roomLabel);
    centerLayout->addWidget(roomTable);

    // Column 3
    QWidget* rightCard = new QWidget();
    rightCard->setStyleSheet(cardStyle());
    QVBoxLayout* rightLayout = new QVBoxLayout(rightCard);
    QLabel* residentsLabel = new QLabel("3. ROOM RESIDENTS (CLICK ROW TO ACTION)");
    residentsLabel->setStyleSheet("color: #c9a84c; font-weight: bold; font-size: 10px; letter-spacing: 1px; background: transparent; border: none;");
    roomStudentsTable = new QTableWidget();
    roomStudentsTable->setColumnCount(3);
    roomStudentsTable->setHorizontalHeaderLabels({"ID", "NAME", "YEAR"});
    roomStudentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    roomStudentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roomStudentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    roomStudentsTable->setStyleSheet(tableStyle());
    roomStudentsTable->verticalHeader()->setVisible(false);
    rightLayout->addWidget(residentsLabel);
    rightLayout->addWidget(roomStudentsTable);

    content->addWidget(leftCard);
    content->addWidget(centerCard);
    content->addWidget(rightCard);
    layout->addLayout(content);

    connect(btnAddDorm,  &QPushButton::clicked, this, &MainWindow::addDormitory);
    connect(btnAddRoom,  &QPushButton::clicked, this, &MainWindow::addRoom);
    connect(btnAssign,   &QPushButton::clicked, this, &MainWindow::assignStudent);
    connect(btnUnassign, &QPushButton::clicked, this, &MainWindow::removeStudentFromRoom);
    connect(btnTransfer, &QPushButton::clicked, this, &MainWindow::transferStudentRoom);

    connect(dormList,          &QListWidget::itemClicked,     this, &MainWindow::onDormitorySelected);
    connect(roomTable,         &QTableWidget::cellClicked,    this, &MainWindow::onRoomSelected);
    connect(roomStudentsTable, &QTableWidget::cellClicked,    this, &MainWindow::onRoomStudentSelected);

    connect(btnRemoveDorm, &QPushButton::clicked, this, [this]() {
        int currentDormRow = dormList->currentRow();
        if (currentDormRow < 0) {
            QMessageBox::warning(this, "No Selection", "Please select a Dormitory from the first column list first!");
            return;
        }

        Dormitory& d = university->getDormitories()[currentDormRow];
        QString dormName = QString::fromStdString(d.getName());

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Dormitory",
                                      QString("Are you sure you want to permanently delete %1?\n"
                                              "All rooms inside will be erased, and living residents will be checked out safely.").arg(dormName),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (university->removeDormitory(dormName.toStdString())) {
                university->saveData("udrms_data.txt");
                dormList->setCurrentRow(-1);
                refreshDormitoryList();
                refreshStudentTable();
                QMessageBox::information(this, "Success", "Dormitory removed successfully.");
            } else {
                QMessageBox::warning(this, "Error", "Failed to remove dormitory.");
            }
        }
    });
}

void MainWindow::setupRestaurantPanel() {
    QVBoxLayout* layout = new QVBoxLayout(restaurantPanel);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QWidget* titleCard = new QWidget();
    titleCard->setStyleSheet(cardStyle());
    titleCard->setFixedHeight(70);
    QHBoxLayout* titleLayout = new QHBoxLayout(titleCard);
    QLabel* icon = new QLabel("🍽");
    icon->setStyleSheet("font-size: 28px; background: transparent; border: none;");
    QLabel* title = new QLabel("RESTAURANT MANAGEMENT");
    title->setStyleSheet("color: #c9a84c; font-size: 22px; font-weight: bold; letter-spacing: 4px; background: transparent; border: none;");
    titleLayout->addWidget(icon);
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    layout->addWidget(titleCard);

    QPushButton* btnSetMenu = new QPushButton("📝   SET DAILY MENU");
    btnSetMenu->setStyleSheet(actionBtnStyle());
    btnSetMenu->setCursor(Qt::PointingHandCursor);
    btnSetMenu->setFixedHeight(38);
    btnSetMenu->setFixedWidth(200);
    layout->addWidget(btnSetMenu);

    QWidget* menuCard = new QWidget();
    menuCard->setStyleSheet(cardStyle());
    QVBoxLayout* menuLayout = new QVBoxLayout(menuCard);
    restaurantList = new QListWidget();
    restaurantList->setStyleSheet(
        "QListWidget { background: transparent; color: #e8dcc8; border: none; font-size: 13px; }"
        "QListWidget::item { padding: 6px; }"
        "QListWidget::item:hover { color: #c9a84c; }"
        );
    menuLayout->addWidget(restaurantList);
    layout->addWidget(menuCard);

    connect(btnSetMenu, &QPushButton::clicked, this, &MainWindow::setMenu);
}

void MainWindow::showStudentPanel()    { refreshStudentTable(); }
void MainWindow::showDormitoryPanel()  { refreshDormitoryList(); }
void MainWindow::showRestaurantPanel() { refreshRestaurantView(); }

void MainWindow::addStudent() {
    bool ok;
    QString id = QInputDialog::getText(this, "Add Student", "Student ID (numbers only):", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty()) return;

    if (!isNumericId(id)) {
        QMessageBox::warning(this, "Invalid ID", "Student ID must contain numbers only!");
        return;
    }
    if (university->findStudent(id.toStdString())) {
        QMessageBox::warning(this, "Error", "Student ID already exists!");
        return;
    }

    QString name = QInputDialog::getText(this, "Add Student", "Full Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    int year = QInputDialog::getInt(this, "Add Student", "Academic Year (1-5):", 1, 1, 5, 1, &ok);
    if (!ok) return;

    university->addStudent(id.toStdString(), name.toStdString(), year);
    refreshStudentTable();
    QMessageBox::information(this, "Success", "Student added successfully!");
}

void MainWindow::onStudentSelected(int row, int column) {
    Q_UNUSED(column);
    studentTable->selectRow(row);
}

void MainWindow::removeStudent() {
    int currentRow = studentTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please click on a student in the table first!");
        return;
    }

    QTableWidgetItem* idItem = studentTable->item(currentRow, 0);
    QTableWidgetItem* nameItem = studentTable->item(currentRow, 1);
    if (!idItem) return;

    QString studentId = idItem->text();
    QString studentName = nameItem ? nameItem->text() : "Selected Student";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion",
                                  QString("Are you sure you want to permanently delete %1 (ID: %2)?").arg(studentName, studentId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (university->removeStudent(studentId.toStdString())) {
            university->saveData("udrms_data.txt");
            refreshStudentTable();
            refreshDormitoryList();
            QMessageBox::information(this, "Success", "Student removed successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Student not found in registry.");
        }
    }
}

void MainWindow::refreshStudentTable() {
    studentTable->setRowCount(0);
    for (Student* s : university->getAllStudents()) {
        int row = studentTable->rowCount();
        studentTable->insertRow(row);
        studentTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(s->getId())));
        studentTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getFullName())));
        studentTable->setItem(row, 2, new QTableWidgetItem(QString::number(s->getAcademicYear())));
        studentTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(s->getDormName())));
        studentTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(s->getRoomNumber())));
        for (int col = 0; col < 5; col++)
            studentTable->item(row, col)->setForeground(QColor("#e8dcc8"));
    }
}

void MainWindow::onDormitorySelected(QListWidgetItem* item) {
    Q_UNUSED(item);
    refreshRoomTable();
}

void MainWindow::onRoomSelected(int row, int column) {
    Q_UNUSED(column);
    roomTable->selectRow(row);
    refreshRoomStudentsTable();
}

void MainWindow::onRoomStudentSelected(int row, int column) {
    Q_UNUSED(column);
    roomStudentsTable->selectRow(row);
}

void MainWindow::refreshDormitoryList() {
    int currentDormRow = dormList->currentRow();
    dormList->clear();
    for (Dormitory& d : university->getDormitories()) {
        dormList->addItem(QString::fromStdString(d.getName()) + " (" + QString::number(d.getTotalOccupancy()) + "/" + QString::number(d.getCapacity()) + ")");
    }
    if (currentDormRow >= 0 && currentDormRow < dormList->count()) {
        dormList->setCurrentRow(currentDormRow);
    } else {
        roomTable->setRowCount(0);
        roomStudentsTable->setRowCount(0);
    }
}

void MainWindow::refreshRoomTable() {
    int currentDormRow = dormList->currentRow();
    if (currentDormRow < 0) {
        roomTable->setRowCount(0);
        return;
    }

    roomTable->setRowCount(0);
    roomStudentsTable->setRowCount(0);
    Dormitory& d = university->getDormitories()[currentDormRow];

    for (const Room& r : d.getRooms()) {
        int row = roomTable->rowCount();
        roomTable->insertRow(row);
        roomTable->setItem(row, 0, new QTableWidgetItem("Room " + QString::number(r.getRoomNumber())));
        roomTable->setItem(row, 1, new QTableWidgetItem(QString::number(r.getOccupancy()) + " / " + QString::number(r.getCapacity())));

        QColor color = r.isFull() ? QColor("#e74c3c") : QColor("#2ecc71");
        roomTable->item(row, 1)->setForeground(color);
        roomTable->item(row, 0)->setForeground(QColor("#e8dcc8"));
    }
}

void MainWindow::refreshRoomStudentsTable() {
    int currentDormRow = dormList->currentRow();
    int currentRoomRow = roomTable->currentRow();
    if (currentDormRow < 0 || currentRoomRow < 0) {
        roomStudentsTable->setRowCount(0);
        return;
    }

    roomStudentsTable->setRowCount(0);
    Dormitory& d = university->getDormitories()[currentDormRow];
    const Room& r = d.getRooms()[currentRoomRow];

    for (Student* s : r.getStudents()) {
        int row = roomStudentsTable->rowCount();
        roomStudentsTable->insertRow(row);
        roomStudentsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(s->getId())));
        roomStudentsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getFullName())));
        roomStudentsTable->setItem(row, 2, new QTableWidgetItem(QString::number(s->getAcademicYear())));

        for (int col = 0; col < 3; col++)
            roomStudentsTable->item(row, col)->setForeground(QColor("#e8dcc8"));
    }
}

void MainWindow::addDormitory() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Dormitory", "Dormitory Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    int cap = QInputDialog::getInt(this, "Add Dormitory", "Capacity:", 20, 1, 500, 1, &ok);
    if (!ok) return;
    university->addDormitory(name.toStdString(), cap);
    refreshDormitoryList();
}

void MainWindow::addRoom() {
    int currentDormRow = dormList->currentRow();
    if (currentDormRow < 0) {
        QMessageBox::warning(this, "Selection Missing", "Please pick a Dormitory from the first column list first!");
        return;
    }
    Dormitory& d = university->getDormitories()[currentDormRow];

    bool ok;
    int roomNum = QInputDialog::getInt(this, "Add Room", QString("Room Number for %1:").arg(QString::fromStdString(d.getName())), 101, 1, 9999, 1, &ok);
    if (!ok) return;
    int cap = QInputDialog::getInt(this, "Add Room", "Room Bed Capacity:", 2, 1, 10, 1, &ok);
    if (!ok) return;

    d.addRoom(roomNum, cap);
    refreshRoomTable();
}

void MainWindow::assignStudent() {
    int currentDormRow = dormList->currentRow();
    int currentRoomRow = roomTable->currentRow();
    if (currentDormRow < 0 || currentRoomRow < 0) {
        QMessageBox::warning(this, "Selection Missing", "Please select both a Dormitory AND a Room first!");
        return;
    }

    Dormitory& d = university->getDormitories()[currentDormRow];
    const Room& r = d.getRooms()[currentRoomRow];

    bool ok;
    QString studentId = QInputDialog::getText(this, "Check-In Student", QString("Enter Student ID to assign to Room %1:").arg(r.getRoomNumber()), QLineEdit::Normal, "", &ok);
    if (!ok || studentId.isEmpty()) return;

    if (university->assignStudentToRoom(studentId.toStdString(), d.getName(), r.getRoomNumber())) {
        university->saveData("udrms_data.txt");
        refreshDormitoryList();
        refreshRoomTable();
        refreshRoomStudentsTable();
        QMessageBox::information(this, "Success", "Student successfully checked into room.");
    } else {
        QMessageBox::warning(this, "Error", "Check-in failed! Ensure student exists and room is not already full.");
    }
}

void MainWindow::removeStudentFromRoom() {
    int studentRow = roomStudentsTable->currentRow();
    int currentDormRow = dormList->currentRow();

    if (studentRow < 0 || currentDormRow < 0) {
        QMessageBox::warning(this, "No Resident Selected", "Please select a student from the 'ROOM RESIDENTS' column table first!");
        return;
    }

    QString sId = roomStudentsTable->item(studentRow, 0)->text();
    QString sName = roomStudentsTable->item(studentRow, 1)->text();
    Dormitory& d = university->getDormitories()[currentDormRow];

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Evict Student", QString("Remove %1 from this dormitory structure completely?").arg(sName), QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    if (university->removeStudentFromDorm(sId.toStdString(), d.getName())) {
        university->saveData("udrms_data.txt");
        refreshDormitoryList();
        refreshRoomTable();
        refreshRoomStudentsTable();
        QMessageBox::information(this, "Evicted", "Student unassigned successfully.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to unassign student.");
    }
}

void MainWindow::transferStudentRoom() {
    int studentRow = roomStudentsTable->currentRow();
    int currentDormRow = dormList->currentRow();

    if (studentRow < 0 || currentDormRow < 0) {
        QMessageBox::warning(this, "No Resident Selected", "Please click on a student resident you want to transfer first!");
        return;
    }

    QString sId = roomStudentsTable->item(studentRow, 0)->text();
    QString sName = roomStudentsTable->item(studentRow, 1)->text();
    Dormitory& d = university->getDormitories()[currentDormRow];

    bool ok;
    int targetRoom = QInputDialog::getInt(this, "Transfer Room", QString("Enter target room number in %1 for %2:").arg(QString::fromStdString(d.getName()), sName), 101, 1, 9999, 1, &ok);
    if (!ok) return;

    if (university->removeStudentFromDorm(sId.toStdString(), d.getName())) {
        if (university->assignStudentToRoom(sId.toStdString(), d.getName(), targetRoom)) {
            university->saveData("udrms_data.txt");
            refreshDormitoryList();
            refreshRoomTable();
            refreshRoomStudentsTable();
            QMessageBox::information(this, "Transferred", "Student transfer completed successfully!");
        } else {
            QMessageBox::critical(this, "Transfer Error", "Target room is full or doesn't exist! Re-assigning to old status.");
            refreshDormitoryList();
            refreshRoomTable();
            refreshRoomStudentsTable();
        }
    }
}

void MainWindow::setMenu() {
    if (university->getDormitories().empty()) {
        QMessageBox::warning(this, "Error", "No dormitories exist!");
        return;
    }
    bool ok;
    QString dormName = QInputDialog::getText(this, "Set Menu", "Dormitory Name:", QLineEdit::Normal, "", &ok);
    if (!ok || dormName.isEmpty()) return;
    Dormitory* dorm = university->findDormitory(dormName.toStdString());
    if (!dorm) { QMessageBox::warning(this, "Error", "Dormitory not found!"); return; }
    QString breakfast = QInputDialog::getText(this, "Set Menu", "Breakfast:", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    QString lunch = QInputDialog::getText(this, "Set Menu", "Lunch:", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    QString dinner = QInputDialog::getText(this, "Set Menu", "Dinner:", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    Restaurant& rest = dorm->getRestaurant();
    rest.setMenu(breakfast.toStdString(), lunch.toStdString(), dinner.toStdString());
    refreshRestaurantView();
    QMessageBox::information(this, "Success", "Menu updated!");
}

void MainWindow::refreshRestaurantView() {
    restaurantList->clear();
    for (Dormitory& d : university->getDormitories()) {
        const Restaurant& r = d.getRestaurant();
        restaurantList->addItem("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        restaurantList->addItem("  🏠  " + QString::fromStdString(d.getName()));
        restaurantList->addItem("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        restaurantList->addItem("  🍳  Breakfast :  " + QString::fromStdString(r.getBreakfast()));
        restaurantList->addItem("  🥗  Lunch     :  " + QString::fromStdString(r.getLunch()));
        restaurantList->addItem("  🍲  Dinner    :  " + QString::fromStdString(r.getDinner()));
        restaurantList->addItem("");
    }
}