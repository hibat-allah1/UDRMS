#include "restaurant.h"
#include <iostream>
using namespace std;

Restaurant::Restaurant(string name) {
    this->name = name;
    this->breakfast = "Not set";
    this->lunch     = "Not set";
    this->dinner    = "Not set";
    this->mealsServedToday = 0;
}

string Restaurant::getName() const { return name; }
string Restaurant::getBreakfast() const { return breakfast; }
string Restaurant::getLunch() const { return lunch; }
string Restaurant::getDinner() const { return dinner; }
int Restaurant::getMealsServedToday() const { return mealsServedToday; }

void Restaurant::setBreakfast(string meal) { breakfast = meal; }
void Restaurant::setLunch(string meal) { lunch = meal; }
void Restaurant::setDinner(string meal) { dinner = meal; }

void Restaurant::setMenu(string b, string l, string d) {
    breakfast = b;
    lunch     = l;
    dinner    = d;
}

void Restaurant::serveMeal() { mealsServedToday++; }
void Restaurant::resetMealCount() { mealsServedToday = 0; }

void Restaurant::displayMenu() const {
    cout << "=== " << name << " Menu ===" << endl;
    cout << "Breakfast : " << breakfast << endl;
    cout << "Lunch     : " << lunch     << endl;
    cout << "Dinner    : " << dinner    << endl;
    cout << "Meals served today: " << mealsServedToday << endl;
}