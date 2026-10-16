#include <mutex>
#include <functional>

class DiningPhilosophers {
private:
    std::mutex forks[5];

public:
    DiningPhilosophers() {}

    void wantsToEat(
        int philosopher,
        std::function<void()> pickLeftFork,
        std::function<void()> pickRightFork,
        std::function<void()> eat,
        std::function<void()> putLeftFork,
        std::function<void()> putRightFork
    ) {
        int left = philosopher;
        int right = (philosopher + 1) % 5;

        std::unique_lock<std::mutex> leftLock(forks[left], std::defer_lock);
        std::unique_lock<std::mutex> rightLock(forks[right], std::defer_lock);
        std::lock(leftLock, rightLock);  // Acquire both without deadlock.

        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();
    }
};
