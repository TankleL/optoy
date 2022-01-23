#ifndef OPTOY_THREADPOOL_H
#define OPTOY_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <stl-preview/proxy>

namespace optoy {
namespace detail::async {
template<class>
struct execute_f;
template<class R, class... Args>
struct execute_f<R(Args...)> : std::facade_operation<
    R(Args...),
    [](const auto& self, Args...) { self(std::forward<Args>...); }> {};
template<class T>
struct task_t : std::facade<execute_f<T>>{};
} // namespace detail::async

template<size_t _ThreadCount>
class static_thread_pool {
public:
    static_thread_pool();
    static_thread_pool(static_thread_pool&&) = default;
    static_thread_pool(const static_thread_pool&) = delete;
    ~static_thread_pool();
    
    template<class T>
    void submit(T&& task);

private:
    struct ctrl_block {
        std::mutex mtx;
        std::condition_variable cond;
        bool shutdown = false;
        std::queue<std::proxy<detail::async::task_t<void()>>> tasks;
    };
    std::shared_ptr<ctrl_block> _ctrl_block;
};

template<size_t _ThreadCount>
inline static_thread_pool<_ThreadCount>::static_thread_pool()
    : _ctrl_block(std::make_shared<static_thread_pool<_ThreadCount>::ctrl_block>()) {
    for (size_t i = 0; i < _ThreadCount; ++i) {
        std::thread([cb = _ctrl_block]() {
            std::unique_lock<std::mutex> lock(cb->mtx);
            for (;;) {
                if (!cb->tasks.empty()) {
                    auto cur = std::move(cb->tasks.front());
                    cb->tasks.pop();
                    lock.unlock();
                    cur.invoke<detail::async::execute_f<void()>>();
                    lock.lock();
                }
                else if (cb->shutdown) {
                    break;
                }
                else {
                    cb->cond.wait(lock);
                }
            }
        }).detach();
    }
}

template<size_t _ThreadCount>
inline static_thread_pool<_ThreadCount>::~static_thread_pool() {
    if ((bool)_ctrl_block) {
        {
            std::scoped_lock<std::mutex> lock(_ctrl_block->mtx);
            _ctrl_block->shutdown = true;
        }
        _ctrl_block->cond.notify_all();
    }
}

template<size_t _ThreadCount>
template<class T>
inline void static_thread_pool<_ThreadCount>::submit(T&& task) {
    {
        std::scoped_lock<std::mutex> lock(_ctrl_block->mtx);
        _ctrl_block->tasks.emplace(&task);
    }
    _ctrl_block->cond.notify_one();
}
} // namespace optoy

#endif // !OPTOY_THREADPOOL_H
