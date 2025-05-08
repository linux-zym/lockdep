#include "lockdep.h"
#include <vector>
#include <set>
#include <iostream>
#include <cassert>
#include <string>
#include <pthread.h>

using std::string, std::cout, std::endl, std::set;
using edge = std::pair<string, string>;

std::set<string>* vertices = new std::set<string>();
std::set<edge>* edges = new std::set<edge>();
thread_local set<string> held_locks;
static pthread_mutex_t GL = PTHREAD_MUTEX_INITIALIZER;

class HoldLock {
    pthread_mutex_t *lk;
public:
    HoldLock(pthread_mutex_t *lock) : lk(lock) { pthread_mutex_lock(lk); }
    ~HoldLock() { pthread_mutex_unlock(lk); }
};

static void check_cycles() {
    assert(pthread_mutex_trylock(&GL) == EBUSY);

    // Floyd-Warshall算法计算传递闭包
    for (const auto& v : *vertices)
        for (const auto& u : *vertices)
            for (const auto& w : *vertices)
                if (edges->count({u, v}) && edges->count({v, w}))
                    edges->insert({u, w});

    // 检测环
    cout << "\nLockdep check:" << endl;
    for (const auto& [u, v] : *edges) {
        cout << "    " << u << " -> " << v << endl;
        if (u == v) 
            cout << "    \033[31m!!! Cycle detected at " << u << "\033[0m" << endl;
    }
}

extern "C" void lock(lock_t *lk) {
    {
        HoldLock h(&GL);
        vertices->insert(lk->name);
        for (const auto& name : held_locks) {
            edge e(name, lk->name);
            if (!edges->count(e)) {
                edges->insert(e);
                check_cycles();
            }
        }
    }
    held_locks.insert(lk->name);
    mutex_lock(&lk->mutex);
}

extern "C" void unlock(lock_t *lk) {
    mutex_unlock(&lk->mutex);
    held_locks.erase(lk->name);
}