/*
 * A269083  Number of n X 2 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 53, u128 n <= 110,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) - a(n-6)
   with a(1)=4, a(2)=11, a(3)=30, a(4)=76, a(5)=191, a(6)=467
   The function memoizes values for efficiency. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;     /* cache[0] unused, cache[1] = a(1) ... */
static size_t cache_cap = 0;    /* current capacity of cache (>= max computed + 1) */
static idx_t  max_computed = 0; /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, return 0 */
    if (n > max_computed) {
        /* enlarge cache if necessary */
        size_t new_cap = (size_t)n + 1;   /* need indices 0..n */
        if (!cache) {
            cache = seq_alloc(new_cap);
            cache_cap = new_cap;
        } else if (new_cap > cache_cap) {
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
        /* fill missing entries */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) cache[i] = 4;
            else if (i == 2) cache[i] = 11;
            else if (i == 3) cache[i] = 30;
            else if (i == 4) cache[i] = 76;
            else if (i == 5) cache[i] = 191;
            else if (i == 6) cache[i] = 467;
            else {
                cache[i] = cache[i-1]*2
                           + cache[i-2]*3
                           - cache[i-3]*2
                           - cache[i-4]*6
                           - cache[i-5]*4
                           - cache[i-6];
            }
        }
        max_computed = n;
    }
    return cache[(size_t)n];
}
