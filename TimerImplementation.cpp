#include <iostream>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>

class Timer {
public:
    Timer() : timerId(0) {}

    int start(int duration, std::function<void(int)> callback) {
        int id = timerId++;
        timers[id] = std::thread([=]() {
            while (true) {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [id, duration, this]() {
                    return !paused || pausedTimerIds.count(id) != 0;
                });

                if (pausedTimerIds.count(id) != 0) {
                    pausedTimerIds.erase(id);
                    lock.unlock();
                    continue;
                }

                lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(duration));

                lock.lock();
                if (timers.count(id) != 0)
                    callback(id);
                else
                    break;
            }
        });
        timers[id].detach();
        return id;
    }

    void stop(int id) {
        if (timers.count(id) != 0) {
            std::unique_lock<std::mutex> lock(mutex);
            pausedTimerIds.erase(id);
            timers.erase(id);
            condition.notify_all();
            std::cout << "Timer " << id << " stopped!" << std::endl;
        }
    }

    void pause(int id) {
        if (timers.count(id) != 0) {
            std::unique_lock<std::mutex> lock(mutex);
            paused = true;
            pausedTimerIds.insert(id);
            std::cout << "Timer " << id << " paused!" << std::endl;
        }
    }

    void resume(int id) {
        if (timers.count(id) != 0) {
            std::unique_lock<std::mutex> lock(mutex);
            pausedTimerIds.erase(id);
            if (pausedTimerIds.empty()) {
                paused = false;
                condition.notify_all();
                std::cout << "Timer " << id << " resumed!" << std::endl;
            }
        }
    }
    std::unordered_map<int, std::thread> timers;
private:
    int timerId;
    //std::unordered_map<int, std::thread> timers;
    std::unordered_set<int> pausedTimerIds;
    std::mutex mutex;
    std::condition_variable condition;
    bool paused = false;
};
//Timer timer;

class PM{
	public:
	Timer timer;
	void callbackFunc(int id);
};

// Example usage
void PM::callbackFunc(int id) {
    std::cout << "Timer " << id << " expired!" << std::endl;
    timer.timers.erase(id);
}
int main() {
    PM PMObj;
    std::function<void(int)> callback = std::bind(&PM::callbackFunc, &PMObj, std::placeholders::_1);
    int timerId1 = PMObj.timer.start(10000, callback); // Start timer 1 for 2 seconds
    int timerId2 = PMObj.timer.start(1000, callback); // Start timer 2 for 3 seconds

    // Do some other work
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    PMObj.timer.pause(timerId1); // Pause the timer
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // Some condition met, so resume the timer
    PMObj.timer.resume(timerId1);
    
    // Do some other work
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    PMObj.timer.stop(timerId1); // Stop the timer

    return 0;
}