#include <unistd.h>
#include <iostream>
#include "thread_pool.hpp"
#include "json_thread_job.hpp"
using namespace tp;
int main() {
    ThreadPoolOptions options;
    options.setThreadCount(std::thread::hardware_concurrency());
    ThreadPool pool(options);
    for (int i = 0;i < 1000;i++) {
        std::string str;
        str.assign(i, 'L');
        json_thread_job json_job;
        json_job.set_json(str);
        pool.post(json_job);
    }
    sleep(1);   // wait threads to join
    return 0;
}
