#include time.h
#include iostream
#include vector
#include thread
#include mutex
#include functional
#include algorithm
#include deque
#include memory
#include chrono
#include atomic
template class T
class sync_deque final {
public
    sync_deque() = default;
	~sync_deque() = default;
	void push_front(const T &val) {
		stdunique_lockstdmutexlk(lock_);
		deque_.push_front(val);
	}
    T pop_front() {
        stdunique_lockstdmutexlk(lock_);
		if (true == deque_.empty()) {
			return nullptr;
		}
		T val = deque_.front();
		deque_.pop_front();
		return val;
	}
	T pop_back() {
        stdunique_lockstdmutexlk(lock_);
		if (true == deque_.empty()) {
			return nullptr;
		}
		T val = deque_.back();
		deque_.pop_back();
		return val;
	}
	inline bool empty() {
		return true == deque_.empty();
	}
    inline size_t get_size() {
        stdunique_lockstdmutexl(lock_);
		return deque_.size();
    }
private
	stddequeTdeque_;
	stdmutex lock_;
};
class worker_t;
using workers_ptr = stdshared_ptrstdvectorstdshared_ptrworker_t;
using task_t = stdfunctionvoid();
class worker_t final {
public
    explicit worker_t(workers_ptr workers, size_t work_num)  workers_(workers) {
        work_num_ = work_num;
        enabled_ = true;
        thd_ = stdthread(stdbind(&worker_texecute, this));
    }
    inline void assign(const task_t &task) {
        queue_.push_front(task);
    }
    inline bool empty() {
        return true == queue_.empty();
    }
    inline task_t steal() {
        return queue_.pop_back();
    }
    void join() {
        enabled_ = false;
        if (thd_.joinable()) {
            stdcout  thread join.  stdendl;
            thd_.join();
        }
    }
private
    void execute_stealed_task() {
        int rand_select = rand() % work_num_;
        auto worker = workers_-at(rand_select);
        stdcout  rand_select   work thread will be selected.  stdendl;
        auto task = worker-steal();
        if (nullptr != task) {
            task();
        }
    }
    void execute() {
        thread_id_ = stdthis_threadget_id();
        stdcout  thread id =   thread_id_  stdendl;
        while (enabled_) {
            if (work_num_ != workers_-size()) {
                stdthis_threadsleep_for(stdchronomilliseconds(100));
                continue;
            }
            task_t task = queue_.pop_front();
			while (task != nullptr) {
				task();
				task = queue_.pop_front();
			}
            bool no_task = stdall_of(workers_-begin(), workers_-end(), [] (stdshared_ptrworker_t worker) { return worker-empty(); });
            if (true == no_task) {
                stdthis_threadsleep_for(stdchronomilliseconds(100));
                continue;
            }
            execute_stealed_task();
        }
    }
private
    size_t work_num_;
    bool enabled_;
    stdthread thd_;
    workers_ptr workers_;
    stdthreadid thread_id_;
    sync_dequetask_tqueue_;
};
class thread_pool {
public
    thread_pool(size_t thread_num = stdthreadhardware_concurrency()) {
        stdsrand(time(nullptr));
        workers_ = stdmake_sharedstdvectorstdshared_ptrworker_t();
        for (int i = 0;i  thread_num;i++) {
            auto worker = stdmake_sharedworker_t(workers_, thread_num);
            workers_-emplace_back(worker);
        }
    }
    ~thread_pool() {
        for (auto worker  workers_) {
            worker-join();
        }
        workers_-clear();
    }
    inline void add_task(const task_t &task) {
        auto worker = get_rand_worker();
        worker-assign(task);
    }
private
    inline stdshared_ptrworker_t get_rand_worker() {
        static size_t size = workers_-size();
        return workers_-at(rand() % size);
    }
private
    workers_ptr workers_;
};
stdatomicintglobal_num;
int main() {
    thread_pool pool;
    for(size_t i = 0; i  100; i++) {
        pool.add_task([] {
            ++global_num;
            stdcout  global num =   global_num.load()  stdendl;            
        });
    }
 
    return 0;
}