/*
 * A348063  Coefficient of x^2 in expansion of n!* Sum_{k=0..n} binomial(x,k).
 * https://oeis.org/A348063
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)^2 * a(n-2) + (-1)^n * (n-2)! initial
 *            values: a(2) = 1, a(3) = 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Values for n < 2 are undefined; return 0 to avoid crashes. */
    if (n < (idx_t)2)
        return (seq_t)0;

    /* Lazy cache allocation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* capacity in elements           */
    static idx_t  cached_n   = 0;   /* highest computed index         */
    static seq_t fact        = 1;   /* (i-2)! for the next i          */

    if (cache == NULL) {
        const size_t init_cap = 4;   /* indices 0..3 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        /* zero out for safety */
        for (size_t i = 0; i < init_cap; ++i) cache[i] = 0;

        /* known base values: a(2)=1, a(3)=0 */
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;

        cache_cap = init_cap;
        cached_n  = 3;   /* we have computed up to index 3 */
        fact      = (seq_t)1;  /* (3-2)! = 1! */
    }

    /* If the requested value is already in the cache, return it. */
    if (n <= cached_n)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend the cache up to n using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* Update factorial: (i-2)! = previous (i-3)! * (i-2) */
        fact *= (seq_t)(i - 2);

        seq_t term1 = cache[i - 1];
        seq_t term2 = ((seq_t)(i - 1) * (seq_t)(i - 1)) * cache[i - 2];
        seq_t val   = term1 + term2;

        /* Add or subtract the factorial depending on the parity of i. */
        if ((i & 1ULL) == 0)   /* i even */
            val += fact;
        else
            val -= fact;

        cache[i] = val;
    }

    cached_n = n;
    return cache[n];
}
