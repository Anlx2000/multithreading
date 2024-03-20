
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

//    ProducerConsumer<int> producerconsumer(200, 5, 5);
//
//    std::this_thread::sleep_for(std::chrono::seconds(5));

    int taskNum = 20;
    TinyThreadPool threadPool;
    std::vector<int> result(taskNum, 0);
    for(int i = 0; i < taskNum; i++){
        threadPool.addTask(task,i, i + 1, std::ref(result[i]));
    }

    this_thread::sleep_for(std::chrono::seconds(10));
    for(auto x:result) std::cout << x << ' ';

}