
#include <iostream>
#include <vector>
#include "ProducerConsumer.h"
#include "ThreadPool.h"
#include "BlockQueue.h"
using namespace std;

void task(int a, int b, int& ret)
{
  //  std::this_thread::sleep_for(std::chrono::seconds(1));
    ret = a + b;
    return;
}

int main()
{

    cout << "test feature1";
  //  std::this_thread::sleep_for(std::chrono::seconds(5));

//    BlockQueue<std::function<void()> > blockQueue(10, 1);
//
//
//    int taskNum = 20;
//    std::vector<int> result(taskNum, 0);
//    for(int i = 0; i < taskNum; i++){
//        cout << "add!\n";
//        blockQueue.addTask(task,i, i + 1, std::ref(result[i]));
//    }
//
//    this_thread::sleep_for(std::chrono::seconds(10));
//
//    for(auto x:result) std::cout << x << ' ';

}