
#pragma once

#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <thread>

template<typename T>
class MutexQueue{
public:
    MutexQueue(){}
    MutexQueue(const MutexQueue &) = delete;
    MutexQueue(MutexQueue &&) = delete;
    MutexQueue operator = (const MutexQueue &) = delete;
    MutexQueue operator = (const MutexQueue &&) = delete;

    void push(T& item){
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Queue.emplace(item);
    }

    void push(T&& item){
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Queue.emplace(std::move(item));
    }


    bool pop(T& item){
        std::unique_lock<std::mutex> lock(m_Mutex);
        if(m_Queue.empty()) return false;
        item = std::move(m_Queue.front());
        m_Queue.pop();
        return true;
    }

    int size(){
        std::unique_lock<std::mutex> lock(m_Mutex);
        return m_Queue.size();
    }
    bool empty(){
        std::unique_lock<std::mutex> lock(m_Mutex);
        return m_Queue.empty();
    }


private:
    std::queue<T> m_Queue;
    std::mutex m_Mutex;
};



// 队列满时addTask会阻塞
template <typename T>
class BlockQueue{
public:
    BlockQueue(int maxSize = 10, int threadNum = 1):m_WorkingThreads(threadNum), m_MaxSize(maxSize){
        runningStatus = true;
        run();
    }
    ~BlockQueue(){
        runningStatus = false;
        m_QueueNotEmptyCV.notify_one();
        m_QueueNotFullCV.notify_all();
        for(auto &t:m_WorkingThreads){
            if(t.joinable()) t.join();
        }

    }
    template <typename Func, typename... Args>
    void addTask(Func && func, Args ... args){
        blockPush(std::bind(std::forward<Func> (func), std::forward<Args>(args)...) );
    }

    void run(){
        for(int i = 0; i <= threadNum; i++){
            m_WorkingThreads[i]=std::thread(&BlockQueue::running, this);
        }
    }

private:
    int m_MaxSize;
    int threadNum;
    std::mutex m_Mutex;
    using taskType = std::function<void()>;
    MutexQueue<taskType> m_Queue;
    std::condition_variable m_QueueNotFullCV;
    std::condition_variable m_QueueNotEmptyCV;
    std::atomic<bool> runningStatus;
    std::vector<std::thread> m_WorkingThreads;

    bool isFull(){
        if(m_Queue.size() >= m_MaxSize) return true;
        return false;
    }

    void running(){
        while(runningStatus){
            std::unique_lock<std::mutex> lock(m_Mutex);
            if(m_Queue.empty() && runningStatus){
                m_QueueNotEmptyCV.wait(lock);
            }
            if(!m_Queue.empty()){
                taskType carryOut;
                m_Queue.pop(carryOut);
                std::cout << "start!!!\n";
                m_QueueNotFullCV.notify_one();
                carryOut();

            }
        }
    }

    void blockPush(taskType && task){
        std::unique_lock<std::mutex> lock(m_Mutex);
        while(isFull() && runningStatus){
            std::cout << "Queue is full! Waiting for m_QueueNotFullCV\n";
            m_QueueNotFullCV.wait(lock);
        }
        if(!isFull()){
            m_Queue.push(task);
            m_QueueNotEmptyCV.notify_one();
        }
    }



};










