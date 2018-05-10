//
// Name: memory
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#include "memory.h"

void MemoryForCache::HandleRequest(uint64_t addr, int bytes, int read, int &hit, int &time) {
    hit = 1;
    time = latency_.hit_latency + latency_.bus_latency;
    stats_.access_counter++;
    stats_.access_time += time;
}
