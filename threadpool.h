
#include "SafeQueue.h"

class ThreadPool{
public:


private:
    using taskType = std::function<void()>;

    std::vector<taskType> m_Threads;
    std::atomic<bool> m_RunningStatus;
    std::mutex m_Mutex;
    std::condition_variable m_cv;
};