/*
 * A287093  a(0) = 0, a(1) = 2; a(2*n) = sopf(a(n)), a(2*n+1) = a(n) + a(n+1),
 *          where sopf() is the sum of the distinct prime factors (A008472).
 * https://oeis.org/A287093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 4922, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sopf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0, a(1) = 2;
 *          a(2*n)   = sopf(a(n))
 *          a(2*n+1) = a(n) + a(n+1)
 * where sopf(x) = sum of distinct prime factors of x
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sopf.h"

/* dynamic cache */
static seq_t *cache = NULL;
static bool *computed = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t newcap = cache_cap ? cache_cap : 4;
    while (newcap <= n) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    computed = (bool *)realloc(computed, newcap * sizeof(bool));
    for (idx_t i = cache_cap; i < newcap; ++i) computed[i] = false;
    cache_cap = newcap;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        computed = (bool *)malloc(cache_cap * sizeof(bool));
        for (idx_t i = 0; i < cache_cap; ++i) computed[i] = false;
        cache[0] = 0; cache[1] = 2;
        computed[0] = computed[1] = true;
    }

    ensure_cache(n);
    if (computed[n]) return cache[n];

    seq_t val;
    if ((n & 1ULL) == 0) {
        seq_t half = a(n >> 1);
        /* half fits in 64 bits for the required range */
        uint64_t half_val = (uint64_t)half;
        val = sopf(half_val);
    } else {
        seq_t left = a(n >> 1);
        seq_t right = a((n >> 1) + 1);
        val = left + right;
    }

    cache[n] = val;
    computed[n] = true;
    return val;
}
