#pragma once

#include <iostream>
#include <queue>
#include <ctime>
#include <mutex>
#include <thread>

#include "order.h"

using std::cout;     using std::thread;
using std::queue;    using std::mutex;
using std::ref;      using std::string;

class Restaurant {
private:
    queue <Order> orders;
    queue <Order> readyMeal;
    int numCopletOrd;

    mutex orderList;
    mutex blockStr;
    mutex readyList;

    bool work;

    void takingOrders(queue <Order>& ord, bool& opened);
    void cooking(queue <Order>& ord, queue <Order>& rM, bool& opened);
    void delivery(queue <Order>& rM, int& count, bool& opened);

    static string outDish(dishType dh);

public:
    Restaurant();
    ~Restaurant();

    void open();
    void close();
};

