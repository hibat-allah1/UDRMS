#include "university.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

University::University(string name) {
    this->name = name;
}

string University::getName() const { return name; }


void University::addStudent(string id, string fullName, int year) {
    allStudents.push_back(new Student(id, fullName, year));
}

bool University::removeStudent(string studentId) {
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i]->getId() == studentId) {
            for (Dormitory& d : dormitories)
                d.removeStudent(studentId);
            delete allStudents[i];
            allStudents.erase(allStudents.begin() + i);
            return true;
        }
    }
    return false;
}

Student* University::findStudent(string studentId) {
    for (Student* s : allStudents)
        if (s->getId() == studentId) return s;
    return nullptr;
}

vector<Student*> University::getAllStudents() const {
    return allStudents;
}

// ── Dormitory management ────────────────────────────────────────
void University::addDormitory(string name, int capacity) {
    dormitories.push_back(Dormitory(name, capacity));
}

bool University::removeDormitory(string name) {
    for (int i = 0; i < (int)dormitories.size(); i++) {
        if (dormitories[i].getName() == name) {
            for (Room& r : dormitories[i].getRooms()) {
                for (Student* s : r.getStudents()) {
                    s->setDormName("Not assigned");
                    s->setRoomNumber("Not assigned");
                }
            }
            dormitories.erase(dormitories.begin() + i);
            return true;
        }
    }
    return false;
}

Dormitory* University::findDormitory(string name) {
    for (int i = 0; i < (int)dormitories.size(); i++) {
        if (dormitories[i].getName() == name) return &dormitories[i];
    }
    return nullptr;
}

vector<Dormitory>& University::getDormitories() {
    return dormitories;
}

// ── Assignment ──────────────────────────────────────────────────
bool University::assignStudentToRoom(string studentId, string dormName, int roomNumber) {
    Student* s = findStudent(studentId);
    Dormitory* d = findDormitory(dormName);
    if (s == nullptr || d == nullptr) return false;
    return d->assignStudent(s, roomNumber);
}

bool University::removeStudentFromDorm(string studentId, string dormName) {
    Dormitory* d = findDormitory(dormName);
    if (d == nullptr) return false;
    return d->removeStudent(studentId);
}

void University::displayAll() const {
    cout << "=== University Status ===" << endl;
}

University::~University() {
    for (Student* s : allStudents) {
        delete s;
    }
}

// ── Persistent Storage System ───────────────────────────────────
void University::saveData(const string& filename) const {
    ofstream out(filename);
    if (!out) return;

    for (Student* s : allStudents) {
        out << "STUDENT," << s->getId() << "," << s->getFullName() << "," << s->getAcademicYear() << "\n";
    }
    for (const Dormitory& d : dormitories) {
        out << "DORM," << d.getName() << "," << d.getCapacity() << "\n";
        for (const Room& r : d.getRooms()) {
            out << "ROOM," << d.getName() << "," << r.getRoomNumber() << "," << r.getCapacity() << "\n";
        }
        const Restaurant& rest = d.getRestaurant();
        out << "MENU," << d.getName() << "," << rest.getBreakfast() << "," << rest.getLunch() << "," << rest.getDinner() << "\n";
    }
    for (Student* s : allStudents) {
        if (s->getDormName() != "Not assigned" && s->getRoomNumber() != "Not assigned") {
            out << "ASSIGN," << s->getId() << "," << s->getDormName() << "," << s->getRoomNumber() << "\n";
        }
    }
}

void University::loadData(const string& filename) {
    ifstream in(filename);
    if (!in) return;

    allStudents.clear();
    dormitories.clear();

    string line;
    vector<string> assignments;

    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string type;
        getline(ss, type, ',');

        if (type == "STUDENT") {
            string id, name, yearStr;
            getline(ss, id,      ',');
            getline(ss, name,    ',');
            getline(ss, yearStr, ',');
            addStudent(id, name, stoi(yearStr));
        }
        else if (type == "DORM") {
            string name, capStr;
            getline(ss, name,   ',');
            getline(ss, capStr, ',');
            addDormitory(name, stoi(capStr));
        }
        else if (type == "ROOM") {
            string dormName, roomStr, capStr;
            getline(ss, dormName, ',');
            getline(ss, roomStr,  ',');
            getline(ss, capStr,   ',');
            Dormitory* d = findDormitory(dormName);
            if (d) d->addRoom(stoi(roomStr), stoi(capStr));
        }
        else if (type == "MENU") {
            string dormName, b, l, d;
            getline(ss, dormName, ',');
            getline(ss, b, ',');
            getline(ss, l, ',');
            getline(ss, d, ',');
            Dormitory* dorm = findDormitory(dormName);
            if (dorm) dorm->getRestaurant().setMenu(b, l, d);
        }
        else if (type == "ASSIGN") {
            assignments.push_back(line);
        }
    }

    for (const string& assignLine : assignments) {
        stringstream ss(assignLine);
        string type, studentId, dormName, roomStr;
        getline(ss, type,      ',');
        getline(ss, studentId, ',');
        getline(ss, dormName,  ',');
        getline(ss, roomStr,   ',');
        assignStudentToRoom(studentId, dormName, stoi(roomStr));
    }
}
