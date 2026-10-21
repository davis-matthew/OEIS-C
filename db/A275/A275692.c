/*
 * A275692  Numbers k such that every rotation of the binary digits of k is
 *          less than k.
 * https://oeis.org/A275692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 8800, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275692: numbers k such that every rotation of the binary
   digits of k is less than k.  The function searches integers
   sequentially, keeping a cache of qualifying values, and returns
   the n‑th member of the sequence (a(1)=0). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------
       Cache of qualifying values
       ---------------------------------------------------------------- */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static idx_t nextk = 0;
    /* Expand the cache until it contains the nth term */
    while (static_cast<idx_t>(cache.size()) < n) {
        seq_t val = static_cast<seq_t>(nextk);
        bool ok = true;
        if (nextk > 1) {
            uint64_t k = static_cast<uint64_t>(nextk);
            unsigned d = 64 - __builtin_clzll(k);
            uint64_t mask = (d == 64) ? ~0ULL : ((1ULL << d) - 1ULL);
            for (unsigned j = 1; j < d; ++j) {
                uint64_t rot = ((k << j) | (k >> (d - j))) & mask;
                if (rot >= k) { ok = false; break; }
            }
        }
        if (ok) cache.push_back(val);
        ++nextk;
    }
    return cache[n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* number of allocated slots */
    static size_t used  = 0;     /* number of elements currently stored */
    static idx_t nextk   = 0;    /* next integer to test */
    while (used < n) {
        seq_t val = (seq_t)nextk;
        bool ok = true;
        if (nextk > 1) {
            uint64_t k   = (uint64_t)nextk;
            unsigned d   = 64 - __builtin_clzll(k);
            uint64_t mask = (d == 64) ? ~0ULL : ((1ULL << d) - 1ULL);
            for (unsigned j = 1; j < d; ++j) {
                uint64_t rot = ((k << j) | (k >> (d - j))) & mask;
                if (rot >= k) { ok = false; break; }
            }
        }
        if (ok) {
            if (used == cap) {
                size_t newcap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* out of memory */
                cache = tmp;
                cap   = newcap;
            }
            cache[used++] = val;
        }
        ++nextk;
    }
    return cache[n - 1];
#endif
}
