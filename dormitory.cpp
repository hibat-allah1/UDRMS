#include "dormitory.h"
#include <iostream>
using namespace std;

Dormitory::Dormitory(string name, int capacity)
    : restaurant(name + " Restaurant") {
    this->name = name;
    this->capacity = capacity;
}

string Dormitory::getName() const { return name; }
int Dormitory::getCapacity() const { return capacity; }

int Dormitory::getTotalOccupancy() const {
    int total = 0;
    for (const Room& r : rooms)
        total += r.getOccupancy();
    return total;
}

void Dormitory::addRoom(int roomNumber, int roomCapacity) {
    rooms.push_back(Room(roomNumber, roomCapacity));
}





bool Dormitory::removeRoom(int roomNumber) {
    for (int i = 0; i < (int)rooms.size(); i++) {
        if (rooms[i].getRoomNumber() == roomNumber) {
            rooms.erase(rooms.begin() + i);
            return true;
        }
    }
    return false;
}

bool Dormitory::assignStudent(Student* student, int roomNumber) {
    Room* room = findRoom(roomNumber);
    if (room == nullptr) return false;
    if (room->isFull()) return false;
    bool success = room->addStudent(student);
    if (success) {
        student->setDormName(name);
    }
    return success;
}

bool Dormitory::removeStudent(string studentId) {
    for (Room& r : rooms) {
        Student* s = r.findStudent(studentId);
        if (s != nullptr) {
            s->setDormName("Not assigned");
            return r.removeStudent(studentId);
        }
    }
    return false;
}

Room* Dormitory::findRoom(int roomNumber) {
    for (Room& r : rooms)
        if (r.getRoomNumber() == roomNumber) return &r;
    return nullptr;
}

Student* Dormitory::findStudent(string studentId) {
    for (Room& r : rooms) {
        Student* s = r.findStudent(studentId);
        if (s != nullptr) return s;
    }
    return nullptr;
}

Restaurant& Dormitory::getRestaurant() { return restaurant; }
const Restaurant& Dormitory::getRestaurant() const { return restaurant; }

vector<Room>& Dormitory::getRooms() { return rooms; }
const vector<Room>& Dormitory::getRooms() const { return rooms; }

void Dormitory::displayRooms() const {
    cout << "=== " << name << " Rooms ===" << endl;
    for (const Room& r : rooms)
        r.display();
}

void Dormitory::display() const {
    cout << "Dormitory: " << name
         << " | Capacity: " << getTotalOccupancy()
         << "/" << capacity << endl;
}