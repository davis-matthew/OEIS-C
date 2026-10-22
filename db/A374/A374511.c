/*
 * A374511  Expansion of 1/(1 - 4*x - 4*x^2)^(5/2).
 * https://oeis.org/A374511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 10; for n >= 2 a(n) = (2*(2*n+3)*a(n-1) +
 *            4*(n+3)*a(n-2))/n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache to handle arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of elements stored */
    static idx_t cache_cap  = 0;        /* allocated capacity */

    /* initialize cache on first call */
    if (cache_cap == 0) {
        cache_cap = (n > 1 ? (2 * n + 1) : 4);  /* initial estimate */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;  /* allocation failure: zero value */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache_size = 2;
    }

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure: zero value */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return it */
    if (n < cache_size) return cache[n];

    /* compute missing values up to index n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)10;
        } else {
            /* recurrence: a(n) = (2*(2*n+3)*a(n-1) + 4*(n+3)*a(n-2))/n */
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t m1 = (seq_t)(2 * i + 3);      /* 2*n+3 as seq_t */
            seq_t m2 = (seq_t)(i + 3);          /* n+3 as seq_t */

            seq_t term1 = m1 * a1;             /* (2*n+3)*a(n-1) */
            term1 *= (seq_t)2;                 /* *2 */

            seq_t term2 = m2 * a2;             /* (n+3)*a(n-2) */
            term2 *= (seq_t)4;                 /* *4 */

            seq_t num = term1 + term2;
            val = num / (seq_t)i;              /* divide by n */
        }
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
