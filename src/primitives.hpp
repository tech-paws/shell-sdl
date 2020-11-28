#pragma once

#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <stddef.h>
#include <string.h>

#include "errors.hpp"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef i32 b32;
typedef i64 b64;

typedef float f32;
typedef double f64;

typedef uintptr_t umm;
typedef intptr_t imm;

typedef size_t MemoryIndex;

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

enum class ResultCase {
    error,
    success
};

struct GeneralError {
    ErrorCode code;
    char message[256];
};

template<typename T, typename E = GeneralError>
struct Result {
    ResultCase resultCase;
    T payload;
    E error;
};

template<typename T, typename E = GeneralError>
inline Result<T> resultCreateError(const E error) {
    Result<T> result;
    result.resultCase = ResultCase::error;
    result.error = error;
    return result;
}

template<typename T>
inline Result<T> resultCreateGeneralError(const ErrorCode errorCode, const char* errorMessage = "") {
    GeneralError error;
    error.code = errorCode;
    strncpy(error.message, errorMessage, 100);
    return resultCreateError<T>(error);
}

template<typename T, typename... Args>
inline Result<T> resultCreateGeneralError(
    const ErrorCode errorCode,
    const char* fmt,
    Args... args
) {
    GeneralError error;
    error.code = errorCode;
    sprintf(&error.message[0], fmt, args...);
    return resultCreateError<T>(error);
}

template<typename T>
inline Result<T> resultCreateSuccess(T payload) {
    Result<T> result;
    result.resultCase = ResultCase::success;
    result.payload = payload;
    return result;
}

template<typename T>
inline bool resultHasError(Result<T> result) {
    return result.resultCase == ResultCase::error;
}

template<typename T>
inline bool resultIsSuccess(Result<T> result) {
    return result.resultCase == ResultCase::success;
}

template<typename T, typename R>
inline Result<T> switchError(Result<R> result) {
    return resultCreateError<T>(result.error);
}

template<typename E, typename T, typename EIN>
inline Result<T, E> mapError(Result<T, EIN> result) {
    return resultCreateError<T>(result.error);
}

template<typename T>
inline T resultGetPayload(Result<T> result) {
    assert(result.resultCase == ResultCase::success);
    return result.payload;
}

template<typename T>
inline T resultUnwrap(Result<T> result) {
    if (result.resultCase != ResultCase::success) {
        // TODO(sysint64): Log
        puts(result.error.message);
        exit(EXIT_FAILURE);
    }

    return result.payload;
}
