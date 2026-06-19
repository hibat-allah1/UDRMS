#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"

class Student : public Person {
private:
    int academicYear;
    string roomNumber;
    string dormName;

public:
    Student(string id, string fullName, int academicYear);

    int getAcademicYear() const;
    string getRoomNumber() const;
    void setRoomNumber(string roomNum);

    string getDormName() const;
    void setDormName(string dorm);

    void display() const override;
};

#endif