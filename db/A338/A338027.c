/*
 * A338027  Lengths of games in optimal play of "subtract-a-square".
 * https://oeis.org/A338027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338027: Lengths of games in optimal play of "subtract‑a‑square" */
/* Recurrence:  a(0)=0; for n>0 let S(n)={s=k² | 1≤k≤⌊√n⌋}.  If any a(n−s) is even,
   a(n)=1+min{a(n−s) | a(n−s) even}.  Otherwise a(n)=1+max{a(n−s)}.   */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square‑root (floor) for 64‑bit unsigned integers */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Static dynamic cache for computed values */
static seq_t *cache   = NULL;   /* pointers into a growing array */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* number of values calculated (>=1) */

/* Ensure that a(0)…a(n) are computed and cached */
static void ensure_up_to(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_len >= target) return;

    /* Allocate / grow array if needed */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (cache_cap < target) {
        size_t new_cap = cache_cap;
        while (new_cap < target) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise a(0) if not done yet */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }

    /* Compute values from cache_len up to target-1 */
    for (size_t i = cache_len; i < target; ++i) {
        idx_t curr = (idx_t)i;
        seq_t min_even = 0;
        bool has_even = false;
        seq_t max_any = 0;
        idx_t max_k = floor_isqrt_u64(curr);

        for (idx_t k = 1; k <= max_k; ++k) {
            idx_t s = k * k;
            seq_t prev = cache[curr - s];
            seq_t val  = prev + 1;   /* a(curr - s) + 1 */

            if ((prev & 1) == 0) {
                if (!has_even || val < min_even) {
                    min_even = val;
                    has_even = true;
                }
            }
            if (val > max_any) max_any = val;
        }

        cache[i] = has_even ? min_even : max_any;
    }

    cache_len = target;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
