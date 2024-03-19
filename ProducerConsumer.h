

#include "SafeQueue.h"
#include <condition_variable>
#include <thread>
#include <vector>



template <typename T>
class ProducerConsumer{
public:
    ProducerConsumer(): m_MaxSize(20), m_ProducerThreads(2), m_ConsumerThreads(2), m_RunningStatus(true) {
        initialize();
    }

    ProducerConsumer(int queuem_MaxSize, int producerNum, int consumerNum): m_MaxSize(queuem_MaxSize), m_ProducerThreads(producerNum), m_ConsumerThreads(consumerNum), m_RunningStatus(true){
        initialize();
    }
    ~ProducerConsumer(){
        m_RunningStatus = false;
        m_QueueNotEmptyCV.notify_all();
        m_QueueNotFullCV.notify_all();

        for(auto& t:m_ProducerThreads){
            if(t.joinable()) t.join();
        }
        for(auto& t:m_ConsumerThreads){
            if(t.joinable()) t.join();
        }


    }

private:
    int m_MaxSize;
    SafeQueue<T> m_Queue;
    std::mutex m_Mutex;

    std::vector<std::thread> m_ProducerThreads;
    std::vector<std::thread> m_ConsumerThreads;


    std::condition_variable m_QueueNotFullCV;
    std::condition_variable m_QueueNotEmptyCV;

    std::atomic<bool> m_RunningStatus;

    void initialize(){
        for(size_t i = 0; i < m_ProducerThreads.size(); i++){
            m_ProducerThreads[i] = std::thread(&ProducerConsumer::producer, this);
        }
        for(size_t i = 0; i < m_ConsumerThreads.size(); i++){
            m_ConsumerThreads[i] = std::thread(&ProducerConsumer::consumer, this);
        }
    }

    bool isFull(){
        if(m_Queue.size() >= m_MaxSize) return true;
        return false;
    }

    void producer(){
        while(m_RunningStatus){
            std::unique_lock<std::mutex> lock(m_Mutex);

            while(isFull() && m_RunningStatus){
                std::cout << "Queue is full! Waiting for m_QueueNotFullCV\n";
                m_QueueNotFullCV.wait(lock);
            }
            T value = 3;
            m_Queue.push(value);
            m_QueueNotEmptyCV.notify_one();
        }
    }
    void consumer()
    {
        while(m_RunningStatus){
            std::unique_lock<std::mutex> lock(m_Mutex);
            while(m_Queue.empty() && m_RunningStatus){
                std::cout << "Queue is empty! Waiting for m_QueueNotEmptyCV\n";
                m_QueueNotEmptyCV.wait(lock);
            }
            T value;
            bool result = m_Queue.pop(value);
            value++;
            if(result)
                std::cout << "Value is:" << value << '\n';
            else std::cout << "Can't get value!\n";
            m_QueueNotFullCV.notify_one();
        }

    }

};