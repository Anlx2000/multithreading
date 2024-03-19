

#include "SafeQueue.h"
#include <condition_variable>
#include <thread>
#include <vector>



template <typename T>
class ProducerConsumer{
public:
    ProducerConsumer(): maxSize(20), producerThreads(2), consumerThreads(2), runningStatus(true) {
        initialize();
    }

    ProducerConsumer(int queueMaxSize, int producerNum, int consumerNum): maxSize(queueMaxSize), producerThreads(producerNum), consumerThreads(consumerNum), runningStatus(true){
        initialize();
    }
    ~ProducerConsumer(){
        runningStatus = false;
        queueNotEmptyCV.notify_all();
        queueNotFullCV.notify_all();

        for(auto& t:producerThreads){
            if(t.joinable()) t.join();
        }
        for(auto& t:consumerThreads){
            if(t.joinable()) t.join();
        }


    }

private:
    int maxSize;
    SafeQueue<T> m_q;
    std::mutex mtx;

    std::vector<std::thread> producerThreads;
    std::vector<std::thread> consumerThreads;


    std::condition_variable queueNotFullCV;
    std::condition_variable queueNotEmptyCV;

    std::atomic<bool> runningStatus;

    void initialize(){
        for(size_t i = 0; i < producerThreads.size(); i++){
            producerThreads[i] = std::thread(&ProducerConsumer::producer, this);
        }
        for(size_t i = 0; i < consumerThreads.size(); i++){
            consumerThreads[i] = std::thread(&ProducerConsumer::consumer, this);
        }
    }

    bool isFull(){
        if(m_q.size() >= maxSize) return true;
        return false;
    }

    void producer(){
        while(runningStatus){
            std::unique_lock<std::mutex> lock(mtx);

            while(isFull() && runningStatus){
                std::cout << "Queue is full! Waiting for queueNotFullCV\n";
                queueNotFullCV.wait(lock);
            }
            T value = 3;
            m_q.push(value);
            queueNotEmptyCV.notify_one();
        }
    }
    void consumer()
    {
        while(runningStatus){
            std::unique_lock<std::mutex> lock(mtx);
            while(m_q.empty() && runningStatus){
                std::cout << "Queue is empty! Waiting for queueNotEmptyCV\n";
                queueNotEmptyCV.wait(lock);
            }
            T value;
            bool result = m_q.pop(value);
            value++;
            if(result)
                std::cout << "Value is:" << value << '\n';
            else std::cout << "Can't get value!\n";
            queueNotFullCV.notify_one();
        }

    }

};