/*
 * A262075  The first of eight consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of seven
 *          consecutive positive integers.
 * https://oeis.org/A262075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 31*a(n-2) + a(n-3) for n > 3 Initial values:
 *            a(1)=105, a(2)=3248, a(3)=97433
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;       /* 1‑based cache of computed terms */
static idx_t  cache_cap = 0;     /* current allocated size */
static idx_t  last_filled = 0;   /* highest index that has been computed */

/* Ensure that the cache can hold indices up to 'n' (inclusive). */
static void ensure_cache(idx_t n)
{
    /* At least handle 3 terms; also we need room for index 0 unused. */
    idx_t need = n + 1;
    if (need < 4) need = 4;

    if (cache_cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();        /* memory allocation failed */
        cache = tmp;
        cache_cap = need;
    }
}

/* Initialise the first three terms if they have not yet been set. */
static void init_first_terms(void)
{
    if (last_filled != 0) return;   /* already initialised */

    cache[1] = (seq_t)105;
    cache[2] = (seq_t)3248;
    cache[3] = (seq_t)97433;
    last_filled = 3;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is defined only for n ≥ 1 */

    ensure_cache(n);
    init_first_terms();

    if (n <= last_filled) return cache[n];

    /* Compute terms from last_filled+1 up to n using the recurrence. */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        cache[i] = (31 * cache[i-1]) - (31 * cache[i-2]) + cache[i-3];
    }
    last_filled = n;
    return cache[n];
}
