#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include <vector>
#include <string>
#include "student.h"
#include "dormitory.h"
using namespace std;

class University {
private:
    string name;
    vector<Student*> allStudents;
    vector<Dormitory> dormitories;

public:
    University(string name);

    string getName() const;

    void addStudent(string id, string fullName, int year);
    bool removeStudent(string studentId);
    Student* findStudent(string studentId);
    vector<Student*> getAllStudents() const;

    void addDormitory(string name, int capacity);
    bool removeDormitory(string name);
    Dormitory* findDormitory(string name);
    vector<Dormitory>& getDormitories();

    bool assignStudentToRoom(string studentId, string dormName, int roomNumber);
    bool removeStudentFromDorm(string studentId, string dormName);

    void displayAll() const;
    ~University();

    void saveData(const string& filename) const;
    void loadData(const string& filename);
};

#endif