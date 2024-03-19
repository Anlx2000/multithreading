
#include <iostream>
#include <vector>
#include "ProducerConsumer.h"


using namespace std;

int main()
{

    ProducerConsumer<int> producerconsumer(200, 5, 5);

    std::this_thread::sleep_for(std::chrono::seconds(5));


}