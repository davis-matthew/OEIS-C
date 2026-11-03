/*
 * A348338  a(n) is the number of distinct numbers of steps required for the
 *          last n digits of integers to repeat themselves by iterating the
 *          map m -> 2*m.
 * https://oeis.org/A348338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + 2*n - ceil(log_5((n+1)/16)), a(0)=1
 * Range:     u8 n <= 15, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8988 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with integer arithmetic and memoisation. */
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute ceil(log_5((n+1)/16)) without floating point.
   For n = 0,1,2 the value is -1; for n >= 3 it is the minimal
   non‑negative k such that 16*5^k >= n+1. */
static inline int ceil_log5_div16(idx_t n)
{
    if (n < 3) /* ceil(log5((n+1)/16)) = -1 for n = 0,1,2 */
        return -1;

    seq_t limit = 16;          /* current value of 16*5^k   */
    int k = 0;
    seq_t target = (seq_t)(n + 1);

    while (limit < target) {
        limit *= 5;
        ++k;
    }
    return k;
}

/* Static cache for memoised results */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
static int initialized = 0;

/* External linkage as requested. */
seq_t a(idx_t n)
{
    /* One‑time allocation of the cache */
    if (!initialized) {
        cache_cap = 1;
        cache = mem_new(seq_t, 1);
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_computed = 0;
        initialized = 1;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        int e = ceil_log5_div16(i);
        long long step = (long long)2 * (long long)i - (long long)e;
        cache[i] = cache[i - 1] + (seq_t)step;
    }
    max_computed = n;

    return cache[n];
}
