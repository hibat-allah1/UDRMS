#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "student.h"
using namespace std;

class Room {
private:
    int roomNumber;
    int capacity;
    vector<Student*> students;

public:
    Room(int roomNumber, int capacity);

    int getRoomNumber() const;
    int getCapacity() const;
    int getOccupancy() const;
    bool isFull() const;

    bool addStudent(Student* student);
    bool removeStudent(string studentId);
    Student* findStudent(string studentId);

    vector<Student*> getStudents() const;
    void display() const;
};

#endif // ROOM_H