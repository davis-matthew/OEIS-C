/*
 * A367071  G.f. satisfies A(x) = 1 + 2*x + 2*x^2*A(x)^2.
 * https://oeis.org/A367071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: A(x) = 2*(1+2*x) / (1+sqrt(1-8*x^2*(1+2*x)))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exact recurrence: (n+2)*a(n) + 8*(-n+1)*a(n-2) + 8*(-2*n+5)*a(n-3) = 0      */
/* Initial values: a(0)=1, a(1)=2, a(2)=2 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed values */
    static seq_t *cache = NULL;    /* pointer to array of seq_t */
    static idx_t size   = 0;       /* number of elements stored */

    /* If we haven't computed up to n, extend the cache */
    if (n >= size)
    {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        size = new_size;

        /* Set initial terms if they are not already present */
        if (size >= 1) cache[0] = (seq_t)1;
        if (size >= 2) cache[1] = (seq_t)2;
        if (size >= 3) cache[2] = (seq_t)2;

        /* Compute remaining terms up to n */
        for (idx_t i = 3; i <= n; ++i)
        {
            seq_t term_n_2 = cache[i - 2];
            seq_t term_n_3 = cache[i - 3];

            /* numerator = 8 * ((i-1)*term_n_2 + (2*i-5)*term_n_3) */
            seq_t part1 = (seq_t)(i - 1) * term_n_2;
            seq_t part2 = (seq_t)(2 * i - 5) * term_n_3;
            seq_t numerator = (seq_t)8 * (part1 + part2);

            /* denominator = i + 2 */
            seq_t denom = (seq_t)(i + 2);

            cache[i] = numerator / denom;
        }
    }

    return cache[n];
}
