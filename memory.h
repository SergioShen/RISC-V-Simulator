//
// Name: memory
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#ifndef CACHE_MEMORY_H
#define CACHE_MEMORY_H

#include "utility.h"
#include "storage.h"

class MemoryForCache : public Storage {
public:
    MemoryForCache() {}

    ~MemoryForCache() {}

    // Main access process
    void HandleRequest(uint64_t addr, int bytes, int read, int &hit, int &time);

private:
    // Memory implement

    DISALLOW_COPY_AND_ASSIGN(MemoryForCache);
};

#endif //CACHE_MEMORY_H
