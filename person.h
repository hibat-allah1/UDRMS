#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
protected:
    string id;
    string fullName;

public:
    Person(string id, string fullName);

    string getId() const;
    string getFullName() const;
    void setFullName(string name);

    virtual void display() const = 0; // pure virtual
    virtual ~Person() {}
};

#endif