//
// Created by XieJin on 2023/2/27.
//

#ifndef SOFTRENDER_TIMER_H
#define SOFTRENDER_TIMER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#define TEST 0

class Timer
{
public:
    Timer(const std::string& func_name) ;
    ~Timer();
private:
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;
    std::string m_func_name;
};

#if (TEST == 1)
void Function(){
    Timer(__func__);
    for(int i = 0; i < 100; ++i){
        std::cout << "Do something!" << std::endl;
    }
}

int main(){
    Function();
    std::cin.get();
}
#endif


#endif //SOFTRENDER_TIMER_H
