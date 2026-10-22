/*
 * A367746  a(1) = 0, a(2) = 1. For n > 2, if a(n-1) is a novel term, a(n) =
 *          |a(n-1) - a(n-2) + 1|, else if a(n-1) is a repeat term, last seen
 *          at a(m), a(n) = n-m-1.
 * https://oeis.org/A367746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 379, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367746
   Recurrence: a(1)=0, a(2)=1.
   For n>2:
        if a(n-1) is novel, a(n) = |a(n-1)-a(n-2)+1|.
        else (a(n-1) has appeared before), let m be the last index
        before n-1 where a(n-1) occurred;  a(n)=n-m-2. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache for previously computed terms (1‑based) */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;
static size_t cache_cap  = 0;

/* For each possible value < current n we keep the most recent
   and the second‑most recent occurrence indices. */
static idx_t *last_occ  = NULL;   /* most recent index of a value   */
static idx_t *prev_occ  = NULL;   /* previous index before that    */
static size_t occ_cap   = 0;      /* allocated capacity (in indices) */

/* Ensure that cache and occurrence tables can hold indices up to n
   (values are always < n). All newly allocated elements are zeroed. */
static void ensure_capacity(idx_t n)
{
    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* zero the new area */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Expand occurrence tables if necessary, values < n */
    if ((size_t)n > occ_cap) {
        size_t new_cap = occ_cap ? occ_cap << 1 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        idx_t *new_last = (idx_t *)realloc(last_occ,
                               (new_cap + 1) * sizeof(idx_t));
        idx_t *new_prev = (idx_t *)realloc(prev_occ,
                               (new_cap + 1) * sizeof(idx_t));
        if (!new_last || !new_prev) exit(EXIT_FAILURE);
        /* zero the new slots */
        for (size_t i = occ_cap + 1; i <= new_cap; ++i) {
            new_last[i] = 0;
            new_prev[i] = 0;
        }
        last_occ = new_last;
        prev_occ = new_prev;
        occ_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety guard */

    ensure_capacity(n);

    /* Compute terms up to n if not already done */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)0;
        } else if (i == 2) {
            val = (seq_t)1;
        } else {
            seq_t prev = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            idx_t idx_prev = (idx_t)prev;

            /* m is the previous occurrence of a(i-1) before i-1 */
            idx_t m = prev_occ[idx_prev];

            if (m == 0) {   /* novel term */
                /* Compute |prev - prev2 + 1| without signed types */
                if (prev >= prev2)
                    val = prev - prev2 + 1;
                else
                    val = prev2 - prev - 1;
            } else {        /* repeat term */
                val = (seq_t)(i - m - 2);
            }
        }

        /* Store the computed term */
        cache[i] = val;

        /* Update occurrence information for the value we just stored */
        idx_t idx_val = (idx_t)val;
        idx_t old_last = last_occ[idx_val];
        prev_occ[idx_val] = old_last;
        last_occ[idx_val] = i;
    }

    cache_size = n;
    return cache[n];
}
