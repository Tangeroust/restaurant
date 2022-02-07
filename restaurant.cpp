#include "restaurant.h"

Restaurant::Restaurant() : work(false), numCopletOrd(0){
    srand(time(nullptr));
}

Restaurant::~Restaurant()
{   }

void Restaurant::open() {
    work = true;

    cout << "\nRestaurant open!\n\n";

    thread getOrders(&Restaurant::takingOrders, this, ref(orders), ref(work));
    getOrders.detach();

    thread cook(&Restaurant::cooking, this, ref(orders), ref(readyMeal), ref(work));
    cook.detach();

    thread dell(&Restaurant::delivery, this, ref(readyMeal), ref(numCopletOrd), ref(work));
    dell.detach();
}

void Restaurant::takingOrders(queue <Order>& ord, bool& opened) {

    int timeOrder = 0;
    Order newOrder;
    dishType dh;

    const int minTimeOrder = 5;
    const int maxTimeOrder = 10;

    while(opened){
        timeOrder = rand() % (maxTimeOrder - minTimeOrder + 1) + minTimeOrder;

        std::this_thread::sleep_for(std::chrono::seconds(timeOrder));

        if(ord.size() < 20)
        {
            dh = (dishType)(rand() % NUMBER);
            newOrder = Order(dh);

            blockStr.lock();
                cout << "\nNew order received #" << newOrder.serialNumb << " (" << outDish(newOrder.dish) << ").";
            blockStr.unlock();

            orderList.lock();
                ord.push(newOrder);
            orderList.unlock();
        }
        else{
            blockStr.lock();
                cout << "\nOrders are temporarily not accepted.";
            blockStr.unlock();
        }
    }
}

string Restaurant::outDish(dishType dh) {
    switch(dh){
        case PIZZA : return "pizza";
        case SOUP  : return "soup";
        case STEAK : return "steak";
        case SALAD : return "salad";
        case SUSHI : return "sushi";

        default: return "none";
    }
}

void Restaurant::cooking(queue <Order>& ord, queue <Order>& rM, bool& opened) {

    int cookTime = 0;
    Order cookOrder;

    const int minCookTime = 5;
    const int maxCookTime = 15;

    while(opened){
        if(ord.empty()){
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        else{
            orderList.lock();
                cookOrder = ord.front();
                ord.pop();
            orderList.unlock();

            cookTime = rand() % (maxCookTime - minCookTime + 1) + minCookTime;
            std::this_thread::sleep_for(std::chrono::seconds(cookTime));

            readyList.lock();
                rM.push(cookOrder);
            readyList.unlock();

            blockStr.lock();
                cout << "\nOrder #" << cookOrder.serialNumb << " (" << outDish(cookOrder.dish) << ") ready for serving.";
            blockStr.unlock();
        }
    }
}

void Restaurant::delivery(queue<Order> &rM, int &count, bool &opened) {

    const int deliveryTime = 30;
    queue <Order> bag;
    Order ordDell;

    while(opened){
        if(rM.empty())
            std::this_thread::sleep_for(std::chrono::seconds(5));
        else
        {
            readyList.lock();
                while(!rM.empty()){
                    ordDell =  rM.front();
                    rM.pop();

                    bag.push(ordDell);
                }
            readyList.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(deliveryTime));

            while(!bag.empty())
            {
                ordDell = bag.front();
                bag.pop();

                blockStr.lock();
                    cout << "\nOrder #" << ordDell.serialNumb << " (" << outDish(ordDell.dish) << ") brought to the customer.";
                blockStr.unlock();

                count++;
            }
        }
    }
}

void Restaurant::close() {
    do{
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }while(numCopletOrd < 10);

    work = false;

    blockStr.lock();
        cout << "\n\nRestaurant close!";
    blockStr.unlock();
}