/*
 * A375427  Expansion of e.g.f. exp(-x * (1 - x)^2) / (1 - x)^3.
 * https://oeis.org/A375427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * n! * Sum_{k=0..n} binomial(2*k-3,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the differential equation of the g.f.): */
/*   a(0)=1, a(1)=2, a(2)=11, a(3)=50,                                */
/*   a_{n+1} = (n+2) a_n + 5 n a_{n-1} - 7 n(n-1) a_{n-2} +          */
/*             3 n(n-1)(n-2) a_{n-3}   for n >= 3.                     */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = (idx_t)-1;  /* index of last computed term */

    /* Handle small indices directly */
    if (n < 4) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)11;
            case 3: return (seq_t)50;
        }
    }

    /* Initialise cache with base values */
    if (cache == NULL) {
        cap  = 64;            /* initial capacity (>=4) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)50;
        last = 3;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cap) {
        while (cap <= (size_t)n)
            cap <<= 1;          /* double capacity to avoid overflow */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Base cases are already in cache; the loop starts at 4 */
        seq_t n1 = (seq_t)(i - 1);
        seq_t n2 = (seq_t)(i - 2);
        seq_t n3 = (seq_t)(i - 3);

        seq_t term1 = (n1 + (seq_t)2) * cache[i - 1];
        seq_t term2 = (seq_t)5 * n1 * cache[i - 2];
        seq_t term3 = (seq_t)7 * n1 * n2 * cache[i - 3];
        seq_t term4 = (seq_t)3 * n1 * n2 * n3 * cache[i - 4];

        /* a_i = term1 + term2 + term4 - term3  (term3 < sum of others) */
        cache[i] = term1 + term2 + term4 - term3;
    }
    last = n;

    return cache[n];
}
