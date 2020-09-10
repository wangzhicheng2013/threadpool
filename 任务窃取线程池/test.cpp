#include <unistd.h>
#include <atomic>
#include "simple_thread_pool.hpp"
std::atomic<int>global_num;
int main() {
    thread_pool pool;
    for(size_t i = 0; i < 100; i++) {
        pool.add_task([] {
            ++global_num;
            std::cout << "global num = " << global_num.load() << std::endl;            
        });
    }
    sleep(1);
 
    return 0;
}