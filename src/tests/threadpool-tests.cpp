#include <gtest/gtest.h>
#include <optoy.hpp>

namespace ot = optoy;
namespace otg = optoy::geo;

TEST(threadpool_tests, basic) {
    const int tp_scale = 10;
    const int tp_task_count = 10000;
    std::mutex mc;
    std::mutex mf;
    std::condition_variable cvf;

    int task_count{0};
    ot::static_thread_pool<8> tp;
    for (int i = 0; i < tp_task_count; ++i) {
        tp.submit([&mc, &cvf, &task_count, tp_task_count]() {
            std::this_thread::yield();
            {
                std::scoped_lock<std::mutex> lc(mc);
                ++task_count;
                if (task_count == tp_task_count) {
                    cvf.notify_one();
                }
            }
        });
    }

    std::unique_lock<std::mutex> f(mf);
    cvf.wait(f);
    ASSERT_EQ(task_count, tp_task_count);
}


