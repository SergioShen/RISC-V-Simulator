//
// Name: cache
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#include "cache.h"
#include "utility.h"

void Cache::HandleRequest(uint64_t addr, int bytes, int read, int &hit, int &time) {
    hit = 0;
    time = 0;
    stats_.access_counter++;
    uint64_t index = (addr >> config_.num_of_bits_block) & ((1 << config_.num_of_bits_index) - 1);
    uint64_t tag = addr >> (config_.num_of_bits_block + config_.num_of_bits_index);

    if (read) {
        DEBUG("\nRead");
    } else {
        DEBUG("\nWrite");
    }

    // Cache hit?
    CacheBlock *target_block = SearchCache(index, tag);
    if (target_block != NULL) {
        // Cache hit
        DEBUG(" cache HIT at %16.16lx, index %lx, tag %lx\n", addr, index, tag);
        hit = 1;
        time += latency_.bus_latency + latency_.hit_latency;
        stats_.access_time += time;
        target_block->access_counter = stats_.access_counter;
        if (!read) {
            if (config_.write_through) {
                // Write to lower cache
                DEBUG("Write through to lower level\n");
                int lower_hit, lower_time;
                lower_->HandleRequest(addr, bytes, read, lower_hit, lower_time);
                time += latency_.bus_latency + lower_time;
                stats_.access_time += latency_.bus_latency;
            } else
                target_block->dirty_ = true; // Set dirty flag
        }
        return;
    } else {
        // Cache miss
        DEBUG(" cache MISS at %16.16lx, index %lx, tag %lx\n", addr, index, tag);
        int lower_hit, lower_time;
        stats_.miss_num++;
        hit = 0;

        // Read or write?
        if (read) {
            // Find a cache block to store data
            CacheBlock *target_block = FindEmptyBlock(index);
            if (target_block == NULL)
                target_block = ChooseVictim(index, time);
            target_block->tag_ = tag;
            target_block->dirty_ = false;
            target_block->valid_ = true;
            target_block->access_counter = stats_.access_counter;

            // Fetch data from lower level
            stats_.fetch_num++;
            lower_->HandleRequest(addr, bytes, 1, lower_hit, lower_time);
            time += latency_.bus_latency + lower_time;
            stats_.access_time += latency_.bus_latency;
        } else {
            // Write allocate or not?
            if (config_.write_allocate) {
                DEBUG("Write allocate from lower level\n");
                // Find a cache block to store data
                CacheBlock *target_block = FindEmptyBlock(index);
                int choose_victim_time;
                if (target_block == NULL) {
                    target_block = ChooseVictim(index, choose_victim_time);
                    time += choose_victim_time;
                }
                target_block->tag_ = tag;
                target_block->dirty_ = true;
                target_block->valid_ = true;
                target_block->access_counter = stats_.access_counter;

                // Fetch data from lower level
                stats_.fetch_num++;
                lower_->HandleRequest(addr, bytes, 1, lower_hit, lower_time);
                time += latency_.bus_latency + lower_time;
                stats_.access_time += latency_.bus_latency;
            } else {
                lower_->HandleRequest(addr, bytes, 0, lower_hit, lower_time);
                time += latency_.bus_latency + lower_time;
                stats_.access_time += latency_.bus_latency;
            }
        }

        if (PrefetchDecision())
            PrefetchAlgorithm();
    }
}

CacheBlock *Cache::SearchCache(uint64_t index, uint64_t tag) {
    for (int i = 0; i < config_.associativity; i++) {
        if (cache_blocks_[index][i].valid_ && cache_blocks_[index][i].tag_ == tag)
            return &cache_blocks_[index][i];
    }
    return NULL;
}

CacheBlock *Cache::FindEmptyBlock(uint64_t index) {
    for (int i = 0; i < config_.associativity; i++) {
        if (!cache_blocks_[index][i].valid_) {
            DEBUG("Empty block FOUND, index %lx, line %lx\n", index, i);
            return &cache_blocks_[index][i];
        }
    }
    DEBUG("Empty block NOT FOUND in index %lx. ", index);
    return NULL;
}

CacheBlock *Cache::ChooseVictim(uint64_t index, int &time) {
    // Choose victim
    time = 0;
    stats_.replace_num++;
    CacheBlock *victim = LRUReplacement(index);
    if (victim->dirty_ && !config_.write_through) {
        // Write to lower cache
        int lower_hit, lower_time;
        uint64_t addr = (victim->tag_ << (config_.num_of_bits_block + config_.num_of_bits_index))
                        + (index << (config_.num_of_bits_block));
        lower_->HandleRequest(addr, config_.block_size, 0, lower_hit, lower_time);
        time += latency_.bus_latency + lower_time;
        stats_.access_time += latency_.bus_latency;
    }
    victim->valid_ = false;
    return victim;
}

CacheBlock *Cache::LRUReplacement(uint64_t index) {
    int min_access_counter = cache_blocks_[index][0].access_counter, min_index = 0;
    for (int i = 1; i < config_.associativity; i++) {
        if (cache_blocks_[index][i].access_counter < min_access_counter) {
            min_access_counter = cache_blocks_[index][i].access_counter;
            min_index = i;
        }
    }
    DEBUG("Victim choosed, index %lx, line %lx\n", index, min_index);
    return &cache_blocks_[index][min_index];
}

bool Cache::PrefetchDecision() {
    return false;
}

void Cache::PrefetchAlgorithm() {
}

Cache::Cache() {
    lower_ = NULL;
    cache_blocks_ = NULL;
}

Cache::~Cache() {
    for (int i = 0; i < config_.set_num; i++)
        delete cache_blocks_[i];
    delete cache_blocks_;
}

void Cache::BuildBlocks() {
    cache_blocks_ = new CacheBlock *[config_.set_num];
    for (int i = 0; i < config_.set_num; i++) {
        cache_blocks_[i] = new CacheBlock[config_.associativity];
        for (int j = 0; j < config_.associativity; j++)
            cache_blocks_[i][j].valid_ = false;
    }
}
