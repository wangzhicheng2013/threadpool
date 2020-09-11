#include <mutex>
#include <deque>
#include <memory>
template <class T>
class sync_deque final {
public:
    sync_deque() = default;
	~sync_deque() = default;
	void push_front(const T &val) {
		std::unique_lock<std::mutex>lk(lock_);
		deque_.push_front(val);
	}
    T pop_front() {
        std::unique_lock<std::mutex>lk(lock_);
		if (true == deque_.empty()) {
			return nullptr;
		}
		T val = deque_.front();
		deque_.pop_front();
		return val;
	}
	T pop_back() {
        std::unique_lock<std::mutex>lk(lock_);
		if (true == deque_.empty()) {
			return nullptr;
		}
		T val = deque_.back();
		deque_.pop_back();
		return val;
	}
	inline bool empty() {
		std::unique_lock<std::mutex>lk(lock_);
		return true == deque_.empty();
	}
    inline size_t get_size() {
        std::unique_lock<std::mutex>l(lock_);
		return deque_.size();
    }
private:
	std::deque<T>deque_;
	std::mutex lock_;
};