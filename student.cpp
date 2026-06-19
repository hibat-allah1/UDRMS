#include "student.h"
#include <iostream>
using namespace std;

Student::Student(string id, string fullName, int academicYear)
    : Person(id, fullName) {
    this->academicYear = academicYear;
    this->roomNumber = "Not assigned";
    this->dormName = "Not assigned";
}

int Student::getAcademicYear() const { return academicYear; }
string Student::getRoomNumber() const { return roomNumber; }
void Student::setRoomNumber(string roomNum) { roomNumber = roomNum; }

string Student::getDormName() const { return dormName; }
void Student::setDormName(string dorm) { dormName = dorm; }

void Student::display() const {
    cout << "ID: " << id
         << " | Name: " << fullName
         << " | Year: " << academicYear
         << " | Dorm: " << dormName
         << " | Room: " << roomNumber << endl;
}