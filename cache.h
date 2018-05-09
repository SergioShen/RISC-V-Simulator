//
// Name: cache
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#ifndef CACHE_CACHE_H
#define CACHE_CACHE_H

#include "utility.h"
#include "storage.h"

typedef struct CacheConfig_ {
    int size;
    int block_size;
    int associativity;
    int set_num; // Number of cache sets
    int write_through; // 0|1 for back|through
    int write_allocate; // 0|1 for no-alc|alc
    int num_of_bits_block;
    int num_of_bits_index;
} CacheConfig;

typedef struct CacheBlock_ {
public:
    bool valid_;
    bool dirty_;
    int access_counter; // Used for LRU replacement
    uint64_t tag_;
} CacheBlock;

class Cache : public Storage {
public:
    Cache();

    ~Cache();

    // Sets & Gets
    void SetConfig(CacheConfig cc) {
        config_ = cc;
        BuildBlocks();
    }

    void GetConfig(CacheConfig cc) { cc = config_; }

    void SetLower(Storage *ll) { lower_ = ll; }

    // Main access process
    void HandleRequest(uint64_t addr, int bytes, int read, int &hit, int &time);

private:
    CacheBlock *SearchCache(uint64_t index, uint64_t tag);

    CacheBlock *FindEmptyBlock(uint64_t index);

    CacheBlock *ChooseVictim(uint64_t index, int &time);

    // Replacement algorithms
    CacheBlock *LRUReplacement(uint64_t index);

    // Prefetching
    bool PrefetchDecision();

    void PrefetchAlgorithm();

    void BuildBlocks();

    CacheConfig config_;
    Storage *lower_;
    CacheBlock **cache_blocks_;
    DISALLOW_COPY_AND_ASSIGN(Cache);
};

#endif //CACHE_CACHE_H
