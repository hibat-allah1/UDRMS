#include "person.h"
#include <iostream>
using namespace std;

Person::Person(string id, string fullName) {
    this->id = id;
    this->fullName = fullName;
}

string Person::getId() const { return id; }
string Person::getFullName() const { return fullName; }
void Person::setFullName(string name) { fullName = name; }