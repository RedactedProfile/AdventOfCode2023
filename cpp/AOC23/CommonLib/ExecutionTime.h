#ifndef EXECUTION_TIMER_H
#define EXECUTION_TIMER_H

#include <chrono>

template<class Resolution = std::chrono::milliseconds>
class ExecutionTimer {
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock>;
private:
    const Clock::time_point mStart = Clock::now();

public:
    ExecutionTimer() = default;
    ~ExecutionTimer() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << "Destructor Elapsed: "
            << std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - mStart)
            << std::endl;
        std::cout << strStream.str() << std::endl;
    }

    inline void stop() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << "Stop Elapsed: "
            << std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - mStart)
            << std::endl;
        std::cout << strStream.str() << std::endl;
    }

}; // ExecutionTimer

#endif // EXECUTION_TIMER_H