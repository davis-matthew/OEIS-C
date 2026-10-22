/*
 * A384315  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for Demos started at 1.
 * https://oeis.org/A384315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^13 * a(n-1) mod (2^26-9317), a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

static const uint64_t MOD_VAL = ((uint64_t)1 << 26) - 9317ULL;
static const uint64_t MULTA   = 8192ULL;

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */

#if defined(__cplusplus)
    /* C++ build: use std::vector for dynamic array */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.push_back((seq_t)1);      /* a(1) = 1 */
    }
    while (cache.size() < n) {
        uint64_t prev = (uint64_t)cache.back();
        uint64_t val  = (prev * MULTA) % MOD_VAL;
        cache.push_back((seq_t)val);
    }
    return cache[n - 1];
#else
    /* C build: use malloc/realloc */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  len   = 0;

    if (cache == NULL) {
        cap = 256;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        len = 1;
    }

    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    while (len < n) {
        uint64_t prev = (uint64_t)cache[len - 1];
        uint64_t val  = (prev * MULTA) % MOD_VAL;
        cache[len++] = (seq_t)val;
    }
    return cache[n - 1];
#endif
}
