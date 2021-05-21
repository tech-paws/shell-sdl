#pragma once

#include "vm.hpp"

static const size_t LOG_BUFFER_SIZE = 1024;

inline void log_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[LOG_BUFFER_SIZE];
    vsprintf(buffer, format, args);
    tech_paws_vm_log_error(buffer);
    va_end(args);
}

inline void log_trace(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[LOG_BUFFER_SIZE];
    vsprintf(buffer, format, args);
    tech_paws_vm_log_trace(buffer);
    va_end(args);
}

inline void log_warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[LOG_BUFFER_SIZE];
    vsprintf(buffer, format, args);
    tech_paws_vm_log_warn(buffer);
    va_end(args);
}

inline void log_debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[LOG_BUFFER_SIZE];
    vsprintf(buffer, format, args);
    tech_paws_vm_log_debug(buffer);
    va_end(args);
}

inline void log_info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[LOG_BUFFER_SIZE];
    vsprintf(buffer, format, args);
    tech_paws_vm_log_info(buffer);
    va_end(args);
}
