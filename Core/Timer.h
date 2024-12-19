#pragma once

#include <vector>
#include <string_view>
#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <cstdio>

#ifdef DEBUG
#define FUNC_PROFILE()      Timer Start(__func__)
#define FUNC_PROFILE_SHOW() TimerManager::GetInstance().Show()

#else
#define FUNC_PROFILE()      static_cast<void>(0);
#define FUNC_PROFILE_SHOW() static_cast<void>(0);

#endif // DEBUG

struct FuncProf {
    std::string_view Name;
    double Time = 0.0;
    uint64_t Call = 0;

    FuncProf(const char* FuncName) : Name(FuncName) {}
    FuncProf(const std::string_view& FuncName) : Name(FuncName) {}
};

inline bool operator< (const FuncProf& lhs, const FuncProf& rhs) { return lhs.Time < rhs.Time; }
inline bool operator> (const FuncProf& lhs, const FuncProf& rhs) { return rhs < lhs; }
inline bool operator<=(const FuncProf& lhs, const FuncProf& rhs) { return !(lhs > rhs); }
inline bool operator>=(const FuncProf& lhs, const FuncProf& rhs) { return !(lhs < rhs); }

// Main Timer
class TimerManager {
public:
    using Container = std::vector<FuncProf>;

public:
    TimerManager& operator=(const TimerManager&) = delete;

    static TimerManager& GetInstance() {
        static TimerManager s_Instance;
        return s_Instance;
    }

    void operator()(const std::string_view& FuncName, double TimeCost, uint64_t CallCount = 1) {
        FuncProf& Prof = _Find(FuncName);
        Prof.Time += TimeCost;
        Prof.Call += CallCount;
    }

    void Show() {
        std::sort(m_Data.begin(), m_Data.end(), std::greater<FuncProf>());
        printf("\n--------------------Function Profile--------------------\n");
        for (const auto& Prof : m_Data) {
            printf("%20.20s %5.0f ms / %10" PRIu64 " call\n", Prof.Name.data(), Prof.Time, Prof.Call);
        }
        printf("--------------------------------------------------------\n");
    }

    ~TimerManager() {
        Show();
    }

private:
    TimerManager() {
        m_Data.reserve(8);
    }

    FuncProf& _Find(const std::string_view& FuncName) {
        auto it = std::find_if(m_Data.begin(), m_Data.end(), [&FuncName](const FuncProf& Prof) {
            return Prof.Name == FuncName;
        });
        if (it == m_Data.end()) {
            return m_Data.emplace_back(FuncName);
        }
        return *it;
    }

private:
    Container m_Data;
};

enum class TimerMode {
    None = 0,
    ReportToManager,
    ReportToConsole
};

// Scope-based Timer
class Timer {
public:
    using ClockType = std::chrono::high_resolution_clock;
    using TimePoint = ClockType::time_point;
    using DurationType = std::chrono::duration<double>;

    Timer(std::string_view Info, TimerMode Mode = TimerMode::ReportToManager) :
        m_Start(ClockType::now()), m_Info(Info), m_Mode(Mode) {}

    double GetElapsedTime() {
        const TimePoint End = ClockType::now();
        const DurationType ElapsedSeconds = End - m_Start;
        return ElapsedSeconds.count() * 1000.0;
    }

    ~Timer() {
        const double Time = GetElapsedTime();
        if (m_Mode == TimerMode::ReportToManager) {
            auto& Manager = TimerManager::GetInstance();
            Manager(m_Info, Time);
        } else if (m_Mode == TimerMode::ReportToConsole) {
            printf("%s %5.0f ms\n", m_Info.data(), Time);
        }
    }
    
private:
    TimePoint m_Start;
    std::string_view m_Info;
    TimerMode m_Mode;
};
