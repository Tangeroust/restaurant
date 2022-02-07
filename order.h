#pragma once

enum dishType {PIZZA = 0, SOUP, STEAK, SALAD, SUSHI, NUMBER, NONE};

struct Order{
    dishType dish;
    int serialNumb;
    static int number;

    Order();
    explicit Order(dishType dh);
};
