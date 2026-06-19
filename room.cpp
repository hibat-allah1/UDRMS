#include "room.h"
#include <iostream>
using namespace std;

Room::Room(int roomNumber, int capacity) {
    this->roomNumber = roomNumber;
    this->capacity = capacity;
}

int Room::getRoomNumber() const { return roomNumber; }
int Room::getCapacity() const { return capacity; }
int Room::getOccupancy() const { return students.size(); }
bool Room::isFull() const { return (int)students.size() >= capacity; }

bool Room::addStudent(Student* student) {
    if (isFull()) return false;
    students.push_back(student);
    student->setRoomNumber(to_string(roomNumber));
    return true;
}

bool Room::removeStudent(string studentId) {
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i]->getId() == studentId) {
            students[i]->setRoomNumber("Not assigned");
            students.erase(students.begin() + i);
            return true;
        }
    }
    return false;
}

Student* Room::findStudent(string studentId) {
    for (Student* s : students)
        if (s->getId() == studentId) return s;
    return nullptr;
}

vector<Student*> Room::getStudents() const { return students; }

void Room::display() const {
    cout << "Room " << roomNumber
         << " | Capacity: " << students.size()
         << "/" << capacity << endl;
    for (Student* s : students)
        s->display();
}