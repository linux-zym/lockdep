#include "lockdep.h"
#include <thread>
#include <iostream>

lock_t lockA = LOCK_INIT;
lock_t lockB = LOCK_INIT;

void thread1() {
    std::cout << "Thread1: Lock A\n";
    lock(&lockA);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 确保死锁发生
    std::cout << "Thread1: Lock B\n";
    lock(&lockB);
    unlock(&lockB);
    unlock(&lockA);
}

void thread2() {
    std::cout << "Thread2: Lock B\n";
    lock(&lockB);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread2: Lock A\n";
    lock(&lockA);
    unlock(&lockA);
    unlock(&lockB);
}

int main() {
    std::thread t1(thread1), t2(thread2);
    t1.join(); 
    t2.join();
    return 0;
}