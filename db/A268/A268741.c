/*
 * A268741  a(n) = 2*a(n - 2) - a(n - 1) for n>1, a(0) = 4, a(1) = 5.
 * https://oeis.org/A268741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 2*a(n-2) - a(n-1), a(0) = 4, a(1) = 5; Recursive
 *            implementation with memoisation
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for memoisation. */
static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated capacity          */
static size_t sz    = 0;   /* number of terms stored     */

/* Grow the cache to accommodate at least n_plus_one elements. */
static void ensure(size_t n_plus_one)
{
    if (cap >= n_plus_one)
        return;

    size_t new_cap = cap ? cap * 2 : 2;
    while (new_cap < n_plus_one)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise base values on first call. */
    if (sz == 0) {
        ensure(2);
        cache[0] = 4;
        cache[1] = 5;
        sz = 2;
    }

    /* Return already computed value if possible. */
    if (n < sz)
        return cache[n];

    /* Ensure enough space and compute missing terms. */
    ensure((size_t)n + 1);
    for (idx_t i = sz; i <= n; ++i) {
        seq_t prev  = cache[i - 1];   /* a(i-1) */
        seq_t prev2 = cache[i - 2];   /* a(i-2) */
        cache[i] = 2 * prev2 - prev;  /* recurrence */
    }
    sz = (size_t)n + 1;
    return cache[n];
}
