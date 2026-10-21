/*
 * A297590  Number of n X 3 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 5 neighboring 1s.
 * https://oeis.org/A297590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 10*a(n-3) + 4*a(n-4) - 8*a(n-5) -
 *            8*a(n-6) a(0)=1, a(1)=3, a(2)=9, a(3)=25, a(4)=69, a(5)=205
 *            (empirical recurrence valid for n>=6)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/seq_alloc.h"   /* seq_alloc(size_t) -> seq_t* */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t*, size_t, size_t) -> seq_t* */

static seq_t *cache = NULL;        /* cache of computed values */
static size_t   cap   = 0;         /* current capacity of cache */
static size_t   last  = (size_t)(-1); /* index of last computed value */

/* Reduce code duplication: store small constant multiplication in seq_t */
static inline seq_t mul_small(seq_t a, unsigned int m)
{
    return a * (seq_t)m;
}

/* Allocate or grow the cache to accommodate index n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial allocation: size at least 1 for a(0) */
        cap = 1;
        cache = seq_alloc(cap);
        cache[0] = 1;     /* a(0) = 1 */
        last = 0;
    }

    /* Grow if needed */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap;
        while (new_cap < need) new_cap = new_cap * 2;
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        switch (i) {
            case 0: cache[0] = 1; break;
            case 1: cache[1] = 3; break;
            case 2: cache[2] = 9; break;
            case 3: cache[3] = 25; break;
            case 4: cache[4] = 69; break;
            case 5: cache[5] = 205; break;
            default: {
                seq_t sum  = cache[i-1];
                sum += mul_small(cache[i-2], 2);
                sum += mul_small(cache[i-3], 10);
                sum += mul_small(cache[i-4], 4);
                seq_t sub = mul_small(cache[i-5], 8);
                sub += mul_small(cache[i-6], 8);
                cache[i] = sum - sub;   /* guaranteed non‑negative */
            }
        }
    }
    last = (size_t)n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
