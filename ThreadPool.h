#pragma once
#include "SafeQueue.h"
#include <thread>
class TinyThreadPool{
public:
    TinyThreadPool(const TinyThreadPool &) = delete;
    TinyThreadPool(const TinyThreadPool &&) = delete;
    TinyThreadPool& operator = (const TinyThreadPool&) = delete;
    TinyThreadPool& operator = (const TinyThreadPool&&) = delete;

    TinyThreadPool(): m_Threads(std::thread::hardware_concurrency()), m_RunningStatus(true){
        initialize();
    }
    TinyThreadPool(int threadNum): m_Threads(threadNum), m_RunningStatus(true){
        initialize();
    }
    ~TinyThreadPool(){
        m_RunningStatus = false;
        m_CV.notify_all();

        for(auto& t:m_Threads){
            if(t.joinable()) t.join();
        }
    }

    template <typename Func, typename... Args>
    void addTask(Func && func, Args ... args){
        std::function<void()> taskWrapper = std::bind(std::forward<Func> (func), std::forward<Args>(args)...);
        m_Queue.push(taskWrapper);
        m_CV.notify_one();
    }


private:
    using taskType = std::function<void()>;

    SafeQueue<taskType> m_Queue;
    std::vector<std::thread> m_Threads;

    std::atomic<bool> m_RunningStatus;
    std::mutex m_Mutex;
    std::condition_variable m_CV;


    void initialize(){
        for(int i = 0; i < m_Threads.size(); i++){
            auto worker = [this, i](){
               while(m_RunningStatus){
                    taskType task;
                    bool isSuccess = false;
                    {
                        std::unique_lock<std::mutex> lock(this->m_Mutex);
                        if(this->m_Queue.empty()){
                            this->m_CV.wait(lock);
                        }
                        isSuccess = this->m_Queue.pop(task);
                    }
                    if(isSuccess){
                        std::cout << "Start running task in worker[ID]" << i << '\n';
                        task();
                        std::cout << "End running task in worker[ID]" << i << '\n';
                    }
               }
            };

            m_Threads[i] = std::thread(worker);
        }
    }
};