#pragma once

#include <sstream>
#include <string_view>
#include <vector>

namespace scp {

inline thread_local std::vector<std::string_view> BackTraceStack;

struct BackTrace {
    ~BackTrace() {
        BackTraceStack.pop_back();
    }
    explicit BackTrace(std::string_view name) {
        BackTraceStack.push_back( name );
    }

    BackTrace(const BackTrace&) 		   = delete;
    BackTrace(BackTrace&&)      		   = delete;
    BackTrace& operator=(const BackTrace&) = delete;
    BackTrace& operator=(BackTrace&&)      = delete;

    operator std::string() {
        std::ostringstream os;
        for (size_t i = 0; i < BackTraceStack.size(); ++i) {
            os << "/" << BackTraceStack[i];
        }

        return os.str();
    }
};


}

#define TRACE ::scp::BackTrace trace(__func__)
