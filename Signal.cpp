#include <signal.h>
#include <iostream>



void handler(int signum)
{
    std::cout << "Get signal:" << signum << std::endl;
}

int main()
{
    signal(SIGALRM, handler);
    raise(SIGALRM);
    std::cout << "Signal sent!\n";

}