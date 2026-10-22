/*
 * A373653  a(n) = Sum_{k=0..floor(n/2)} binomial(3*n-5*k-1,k).
 * https://oeis.org/A373653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n-5*k-1,k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-4) + a(n-6) for n > 6 */
/* Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=7, a(5)=16, a(6)=34 */

#include <stdlib.h>

/* Cached values of the sequence for efficient repeated calls. */
static seq_t *cache = NULL;        /* array of computed terms */
static size_t cap = 0;             /* allocated capacity of 'cache' */
static size_t max_computed = 0;    /* highest index that has been computed */

/* Initial values for indices 0..6 */
static const seq_t base_vals[7] = {1, 1, 1, 4, 7, 16, 34};

/* Main function required by the interface */
seq_t a(idx_t n)
{
    /* First-time initialization */
    if (cache == NULL) {
        /* Allocate enough space for the first 7 terms */
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* out of memory; unlikely in tests */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = base_vals[i];
        cap = 7;
        max_computed = 6;
    }

    /* If the requested n is already known, return it */
    if ((size_t)n < cap)
        return cache[(size_t)n];

    /* Expand the cache to accommodate index n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;      /* out of memory */
    cache = tmp;
    cap = new_cap;

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1) + 3*a(i-2) - 3*a(i-4) + a(i-6) */
        seq_t term = cache[i - 1]
                   + (seq_t)3 * cache[i - 2]
                   + cache[i - 6];
        term -= (seq_t)3 * cache[i - 4];
        cache[i] = term;
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
