/*
 * A349182  a(0) = 1; for n>0, a(n) is the smallest positive integer such that
 *          |n - a(n-1)| + a(n) is a square.
 * https://oeis.org/A349182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = smallest positive integer k such that |n - a(n-1)| + k is a perfect square. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Memoised cache of computed values */
static seq_t *cache = NULL;          /* array holding terms a(0) .. a(max_cache) */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t cache_max = 0;          /* highest index currently stored */
static bool initialized = false;

/* Compute a(n) for any n >= 0, with memoisation. */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) on first call */
    if (!initialized) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache_max = 0;
        initialized = true;
    }

    /* If value already computed, return it immediately */
    if (n <= cache_max)
        return cache[n];

    /* Ensure we have enough space for up to n */
    if (n > cache_cap - 1)
        ensure_cache(n, &cache, &cache_cap);

    /* Compute missing terms sequentially */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev_val = cache[i - 1];

        /* Difference |i - prev_val| */
        seq_t i_seq = (seq_t)i;
        seq_t diff = (i_seq >= prev_val) ? (i_seq - prev_val) : (prev_val - i_seq);

        /* Find next perfect square greater than diff */
        seq_t target = diff + (seq_t)1;          /* diff + k where k ≥ 1 */
        seq_t r = floor_sqrt(target);           /* floor sqrt of target */
        seq_t square;
        if (r * r == target) {
            square = target;                    /* target itself is a square */
        } else {
            seq_t r_next = r + (seq_t)1;
            square = r_next * r_next;
        }

        /* Minimal k that makes diff + k a square */
        seq_t k = square - diff;
        cache[i] = k;
    }

    cache_max = n;
    return cache[n];
}
