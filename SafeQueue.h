#pragma once
#include <iostream>
#include <queue>

template <typename T>
class SafeQueue{
public:
    SafeQueue() = default;
    ~SafeQueue() = default;

    SafeQueue(const SafeQueue && ot) = delete;
    SafeQueue(SafeQueue && ot) = delete;
    SafeQueue(const SafeQueue & ot) = delete;
    SafeQueue& operator = (const SafeQueue && ot) = delete;
    SafeQueue& operator = (const SafeQueue & ot) = delete;

    bool empty(){
        std::unique_lock<std::mutex> lock(m_Mutex);
        return m_Queue.empty();
    }

    int size(){
        std::unique_lock<std::mutex> lock(m_Mutex);
        return m_Queue.size();
    }
    void push(T& value){
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Queue.emplace(value);
    }

    void push(T&& value){
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Queue.emplace(std::move(value));
    }

    bool pop(T& value){
        std::unique_lock<std::mutex> lock(m_Mutex);
        if(m_Queue.empty()) return false;
        else{
            value = std::move(m_Queue.front());
            m_Queue.pop();
            return true;
        }
    }

private:
    std::queue<T> m_Queue;
    std::mutex m_Mutex;
};