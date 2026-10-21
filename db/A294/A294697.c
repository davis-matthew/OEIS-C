/*
 * A294697  Number of permutations of [n] avoiding {1342, 2143, 3412}.
 * https://oeis.org/A294697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 31*a(n-2) + 52*a(n-3) - 45*a(n-4) + 19*a(n-5)
 *            - 3*a(n-6) (for n > 5). Initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=6, a(4)=21, a(5)=73
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache     = NULL;  /* array of computed terms  */
static size_t cache_cap = 0;     /* current capacity of the array */
static size_t cache_len = 0;     /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* static table for the first six terms */
    static const seq_t init[6] = {(seq_t)1, 1, 2, 6, 21, 73};

    if (n < 6)            /* use the explicit initial values */
        return init[n];

    /* --- initialise the memoisation array if necessary --- */
    if (cache_len == 0) {
        size_t cap = 16;
        if (cap < 6) cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                  /* fatal error */
        for (size_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_len = 6;
        cache_cap = cap;
    }

    /* If we already have enough terms, just return. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure that the array can hold up to n+1 terms.  Resize if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap < 16) new_cap = 16;
        while ((size_t)n >= new_cap) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                           /* fatal error */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence uses only previously computed terms */
        seq_t pos = 9 * cache[i-1] + 52 * cache[i-3] + 19 * cache[i-5];
        seq_t sub = 31 * cache[i-2] + 45 * cache[i-4] + 3 * cache[i-6];
        cache[i]  = pos - sub;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
