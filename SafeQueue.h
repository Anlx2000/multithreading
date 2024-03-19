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
        std::unique_lock<std::mutex> lock(mtx);
        return q.empty();
    }

    int size(){
        std::unique_lock<std::mutex> lock(mtx);
        return q.size();
    }
    void push(T& value){
        std::unique_lock<std::mutex> lock(mtx);
        q.emplace(value);
    }

    void push(T&& value){
        std::unique_lock<std::mutex> lock(mtx);
        q.emplace(std::move(value));
    }

    bool pop(T& value){
        std::unique_lock<std::mutex> lock(mtx);
        if(q.empty()) return false;
        else{
            value = std::move(q.front());
            q.pop();
            return true;
        }
    }

private:
    std::queue<T> q;
    std::mutex mtx;
};