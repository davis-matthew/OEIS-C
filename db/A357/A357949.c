/*
 * A357949  a(n) = Sum_{k=0..floor(n/4)} (n-3*k)!/k!.
 * https://oeis.org/A357949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (n-3*k)!/k! Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=25, a(5)=122, a(6)=726. For n>6 the
 *            following recurrence is used: a(n) =
 *            (n-1)*a(n-1)+(n-2)*a(n-2)+(n-3)*a(n-3)+(n-6)*a(n-4)
 *            -3*a(n-5)-3*a(n-6)-3*a(n-7)+4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoization cache */
    static seq_t *cache     = NULL;
    static size_t  cache_cap = 0;
    static idx_t   max_comp  = 0;   /* largest computed index (inclusive) */

    /* Initialise cache and first seven terms on first call */
    if (cache == NULL) {
        cache_cap = 32;                                 /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                    /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)122;
        cache[6] = (seq_t)726;
        max_comp = 6;
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1; /* double until enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                     /* allocation failure */
        cache_cap = new_cap;
    }

    /* Return if already computed */
    if (n <= max_comp) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* All required previous terms are already computed at this point */
        seq_t val = 0;

        /* Positive contributions */
        val += ((seq_t)(i - 1)) * cache[i - 1];
        val += ((seq_t)(i - 2)) * cache[i - 2];
        val += ((seq_t)(i - 3)) * cache[i - 3];
        val += ((seq_t)(i - 6)) * cache[i - 4];

        /* Negative contributions */
        val -= ((seq_t)3) * cache[i - 5];
        val -= ((seq_t)3) * cache[i - 6];
        val -= ((seq_t)3) * cache[i - 7];

        /* Final constant term */
        val += (seq_t)4;

        cache[i] = val;
    }

    max_comp = n;
    return cache[n];
}
