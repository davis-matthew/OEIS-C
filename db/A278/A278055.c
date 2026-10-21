/*
 * A278055  Relative of Hofstadter Q-sequence: a(1) = 1, a(2) = 2, a(3) = 3,
 *          a(4) = 4, a(5) = 5; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))
 *          for n > 5.
 * https://oeis.org/A278055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A278055
    Relative of Hofstadter Q-sequence:
    a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5
    a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))   for n > 5
    (indices in the recurrence are 1‑based)                      */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *a_cache = NULL;          /* allocated array */
static size_t a_cache_cap  = 0;        /* number of elements allocated */
static size_t a_cache_len  = 0;        /* number of terms already computed */

/* Ensure that a_cache has at least n elements allocated */
static void ensure_cache_capacity(idx_t n)
{
    if (a_cache_cap >= (size_t)n)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    if (new_cap < (size_t)n)
        new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* out of memory */
    a_cache = new_cache;
    a_cache_cap = new_cap;
}

/* Compute a(n) for n >= 1.  Results are memoised in a_cache. */
seq_t a(idx_t n)
{
    if (n == 0u)            /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Grow cache if necessary */
    ensure_cache_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        if (i <= 5u) {
            /* Base values, 1‑based */
            a_cache[i - 1] = (seq_t)i;
        } else {
            /* Use recurrence: a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) */
            idx_t idx1 = i - (idx_t)a_cache[i - 2]; /* a(n-1) stored at a_cache[i-2] */
            idx_t idx2 = i - (idx_t)a_cache[i - 3]; /* a(n-2) stored at a_cache[i-3] */
            idx_t idx3 = i - (idx_t)a_cache[i - 4]; /* a(n-3) stored at a_cache[i-4] */
            seq_t val = a_cache[idx1 - 1] + a_cache[idx2 - 1] + a_cache[idx3 - 1];
            a_cache[i - 1] = val;
        }
    }
    a_cache_len = n;          /* all terms up to n are now computed */
    return a_cache[n - 1];
}
