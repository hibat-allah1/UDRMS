#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
using namespace std;

class Restaurant {
private:
    string name;
    string breakfast;
    string lunch;
    string dinner;
    int mealsServedToday;

public:
    Restaurant(string name);

    string getName() const;
    string getBreakfast() const;
    string getLunch() const;
    string getDinner() const;
    int getMealsServedToday() const;

    void setBreakfast(string meal);
    void setLunch(string meal);
    void setDinner(string meal);
    void setMenu(string breakfast, string lunch, string dinner);

    void serveMeal();
    void resetMealCount();

    void displayMenu() const;
};

#endif // RESTAURANT_H