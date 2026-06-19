#ifndef DORMITORY_H
#define DORMITORY_H

#include <vector>
#include <string>
#include "room.h"
#include "restaurant.h"
using namespace std;

class Dormitory {
private:
    string name;
    int capacity;
    vector<Room> rooms;
    Restaurant restaurant;

public:
    Dormitory(string name, int capacity);

    string getName() const;
    int getCapacity() const;
    int getTotalOccupancy() const;

    void addRoom(int roomNumber, int roomCapacity);
    bool removeRoom(int roomNumber);

    bool assignStudent(Student* student, int roomNumber);
    bool removeStudent(string studentId);

    Room* findRoom(int roomNumber);
    Student* findStudent(string studentId);

    Restaurant& getRestaurant();
    const Restaurant& getRestaurant() const;

    vector<Room>& getRooms();
    const vector<Room>& getRooms() const;

    void displayRooms() const;
    void display() const;
};

#endif