#include "order.h"

int Order::number = 1;

Order::Order() : dish(NONE), serialNumb(0)
{   }

Order::Order(dishType dh) : dish(dh), serialNumb(number++)
{   }

