/*
 * A375562  Expansion of e.g.f. 1 / (1 + x * log(1 - x^3)).
 * https://oeis.org/A375562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (n-3*k)! * |Stirling1(k,n-3*k)| /
 *            k!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Compute a(n) for sequence A375562.
 *  - Uses a cache so that any order of calls works.
 *  - Implements the recurrence derived from the defining formula:
 *
 *    a(n) = Σ_{j=1..⌊(n-1)/3⌋} ( n! / ( (n-(3*j+1))! * j ) ) * a[n-(3*j+1)]
 *
 *    The factor n! / (n-(3*j+1))! is the product of (3*j+1) consecutive
 *    integers: n*(n-1)*...*(n-3*j).  Division by j is exact.
 *  - All arithmetic is done in seq_t, which is an unsigned type that may be
 *    arbitrary‑precision.
 */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;        /* array of values */
    static size_t cache_cap = 0;       /* allocated size */
    static size_t cache_used = 0;      /* number of computed values (0..cache_used-1) */

    /* ensure that the cache can hold at least n+1 entries */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return cached value */
    if ((size_t)n < cache_used)
        return cache[(size_t)n];

    /* compute values from cache_used up to n */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        if (i == 0) {           /* a(0) = 1 */
            cache[0] = 1;
            continue;
        }

        size_t jmax = (i - 1) / 3;   /* floor((i-1)/3) */
        seq_t sum = 0;

        seq_t prod = 1;
        if (jmax >= 1) {
            /* start prod with the first (3*jmax+1) terms for j=1 */
            for (size_t t = 0; t < 4; ++t)    /* 4 = 3*1+1 */
                prod *= (seq_t)(i - t);
        }

        for (size_t j = 1; j <= jmax; ++j) {
            size_t m = i - (3 * j + 1);         /* index for a[m] */
            seq_t term = prod;
            term /= (seq_t)j;                   /* divide by j */
            term *= cache[m];                   /* multiply by a[m] */
            sum += term;

            /* prepare prod for the next j (if any) */
            if (j < jmax) {
                prod *= (seq_t)(i - (3 * j + 1));
                prod *= (seq_t)(i - (3 * j + 2));
                prod *= (seq_t)(i - (3 * j + 3));
            }
        }

        cache[i] = sum;
    }

    cache_used = (size_t)n + 1;
    return cache[(size_t)n];
}
