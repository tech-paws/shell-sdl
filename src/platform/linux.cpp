#include "platform.hpp"
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

u8* platform_alloc(MemoryIndex size) {
    auto base = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (base == MAP_FAILED) {
        return nullptr;
    }
    else {
        memset(base, 0, size);
        return (u8*) base;
    }
}
