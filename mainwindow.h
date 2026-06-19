#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include "university.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class BackgroundWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Student slots
    void addStudent();
    void removeStudent();
    void refreshStudentTable();
    void onStudentSelected(int row, int column);

    // Dormitory slots
    void addDormitory();
    void addRoom();
    void assignStudent();
    void removeStudentFromRoom();
    void transferStudentRoom();
    void refreshDormitoryList();
    void refreshRoomTable();
    void refreshRoomStudentsTable();
    void onDormitorySelected(QListWidgetItem* item);
    void onRoomSelected(int row, int column);
    void onRoomStudentSelected(int row, int column);

    // Restaurant slots
    void setMenu();
    void refreshRestaurantView();

    // Navigation
    void showStudentPanel();
    void showDormitoryPanel();
    void showRestaurantPanel();

private:
    Ui::MainWindow *ui;
    University* university;

    // Panels
    QWidget* studentPanel;
    QWidget* dormitoryPanel;
    QWidget* restaurantPanel;

    // Student panel widgets
    QTableWidget* studentTable;

    // Dormitory panel widgets
    QListWidget*  dormList;
    QTableWidget* roomTable;
    QTableWidget* roomStudentsTable;

    // Restaurant panel widgets
    QListWidget* restaurantList;

    void setupUI();
    void setupStudentPanel();
    void setupDormitoryPanel();
    void setupRestaurantPanel();
};

#endif // MAINWINDOW_H