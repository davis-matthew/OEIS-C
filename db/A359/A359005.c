/*
 * A359005  Jane Street's infinite sidewalk's greedy walk.
 * https://oeis.org/A359005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0, let curr=a(n-1). If back = curr - floor(curr/2)
 *            - 1 has not been visited, then a(n)=back; otherwise a(n)=curr +
 *            floor(curr/2) + 1. (This is the greedy walk on Jane Street's
 *            sidewalk.)
 * Range:     u8 n <= 93, u16 n <= 3663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cached sequence values a(0)..a(cache_len-1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Append a new value to the cache, expanding if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Extend the cache so that it contains terms up to and including index n. */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        cache_append((seq_t)0);           /* a(0) */
    }
    while (cache_len <= (size_t)n) {
        seq_t curr = cache[cache_len - 1];

        bool can_back = false;
        seq_t back = 0;
        if (curr > (seq_t)0) {
            back = curr - curr / (seq_t)2 - (seq_t)1;
            can_back = true;
        }

        bool back_visited = false;
        if (can_back) {
            for (size_t i = 0; i < cache_len; ++i) {
                if (cache[i] == back) {
                    back_visited = true;
                    break;
                }
            }
        }

        seq_t forward = curr + curr / (seq_t)2 + (seq_t)1;
        seq_t next = can_back && !back_visited ? back : forward;

        cache_append(next);
    }
}

/* Public accessor returning a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
