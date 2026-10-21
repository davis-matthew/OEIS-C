/*
 * A266505  a(n) = 2*a(n - 2) + a(n - 4) with a(0) = -1, a(1) = 1, a(2) = 3,
 *          a(3) = 5.
 * https://oeis.org/A266505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4); a(0) = -1, a(1) = 1, a(2) = 3, a(3) =
 *            5
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 48, i64 n <= 98, i128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t  max_computed = 0;   /* largest n with cache[n] valid   */
static size_t capacity   = 0;     /* number of slots allocated      */

/* Ensure the cache holds at least indices 0..n inclusive. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (capacity < need) {
        seq_t *new_cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!new_cache) abort();  /* allocation failure */
        cache = new_cache;
        capacity = need;
    }
}

seq_t a(idx_t n)
{
    /* First call – initialise base cases */
    if (!cache) {
        capacity = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!cache) abort();
        cache[0] = -1;
        cache[1] =  1;
        cache[2] =  3;
        cache[3] =  5;
        max_computed = 3;
    }

    /* If n is already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Grow the array if necessary */
    if (n >= capacity)
        ensure_cache(n);

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i>=4 the recurrence applies */
        if (i >= 4)
            cache[i] = cache[i-2] * 2 + cache[i-4];
    }

    max_computed = n;
    return cache[n];
}
