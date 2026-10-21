/*
 * A298401  a(n) = a(n-1) + a(n-2) - n*a(floor(n/2)), where a(0) = 1, a(1) =
 *          2, a(2) = 3.
 * https://oeis.org/A298401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-2) - n*a(floor(n/2)), a(0) = 1, a(1) = 2,
 *            a(2) = 3 This function memoizes the sequence so it works for any
 *            order of calls and remains O(n) for n up to about 10 000
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 42, i64 n <= 88, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;   /* cached values */
static size_t  cache_cap = 0;     /* allocated size of seq_cache */
static idx_t   max_computed = (idx_t)-1; /* highest index for which we have a value */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    /* Double to reduce frequent reallocs */
    if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failed */
    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Public function computing the nth term of A298401. */
seq_t a(idx_t n)
{
    /* Lazy initialization of base cases */
    if (seq_cache == NULL) {
        size_t init_cap = 3;          /* covers indices 0,1,2 */
        seq_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq_cache)
            abort();
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_cache[2] = (seq_t)3;
        cache_cap   = init_cap;
        max_computed = 2;
    }

    if (n <= max_computed)
        return seq_cache[n];

    /* Ensure the cache is large enough */
    ensure_capacity(n);

    /* Compute missing terms sequentially up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev1 = seq_cache[i - 1];
        seq_t prev2 = (i >= 2) ? seq_cache[i - 2] : (seq_t)0;
        seq_t term  = prev1 + prev2 - (seq_t)i * seq_cache[i / 2];
        seq_cache[i] = term;
    }
    max_computed = n;
    return seq_cache[n];
}
