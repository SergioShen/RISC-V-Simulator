//
// Name: config
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#ifndef CACHE_CONFIG_H
#define CACHE_CONFIG_H

#include "cache.h"

StorageLatency get_memory_latency();

StorageLatency get_l1_cache_latency();

CacheConfig get_l1_cache_config();

StorageStats get_zero_stats();

StorageLatency get_l2_cache_latency();

StorageLatency get_l3_cache_latency();

CacheConfig get_l2_cache_config();

CacheConfig get_l3_cache_config();

#endif //CACHE_CONFIG_H
