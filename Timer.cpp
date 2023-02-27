//
// Created by XieJin on 2023/2/27.
//

#include "Timer.h"

Timer::Timer(const std::string &func_name) : m_func_name(func_name){
    start = std::chrono::steady_clock::now();
}

Timer::~Timer()
{
    end = std::chrono::steady_clock::now();
    duration = end - start;
    float ms = duration.count() * 1000.0f;
    std::cout << m_func_name <<" took" << ms << " ms" << std::endl;
}