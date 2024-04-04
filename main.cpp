
#include <iostream>
#include <vector>
#include "ProducerConsumer.h"
#include "ThreadPool.h"

using namespace std;

void task(int a, int b, int& ret)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ret = a + b;
    return;
}

int main()
{

    cout << "Feature2 test";
}