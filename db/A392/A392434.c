/*
 * A392434  a(n) = Sum_{k=0..floor(2*n/5)} binomial(3*k,2*n-5*k).
 * https://oeis.org/A392434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n‑3) + 2*a(n‑4) + a(n‑5) - 3*a(n‑6) + 3*a(n‑7) -
 *            a(n‑8) for n ≥ 9 Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=3,
 *            a(4)=1, a(5)=1, a(6)=15, a(7)=15, a(8)=10
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 70, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the nth term of sequence A392434, indexed from 0 */
seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated capacity (number of elements) */
    static size_t len   = 0; /* number of elements currently stored */

    /* Ensure the cache is large enough to hold values up to index n */
    if (len == 0) {
        /* initialize the first nine values */
        size_t init_cap = 8;
        cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)0; /* a(1) */
        cache[2] = (seq_t)0; /* a(2) */
        cache[3] = (seq_t)3; /* a(3) */
        cache[4] = (seq_t)1; /* a(4) */
        cache[5] = (seq_t)1; /* a(5) */
        cache[6] = (seq_t)15;/* a(6) */
        cache[7] = (seq_t)15;/* a(7) */
        cache[8] = (seq_t)10;/* a(8) */
        cap = init_cap + 1;  /* capacity now 9 */
        len = 9;             /* we have filled 9 elements */
    }

    /* If n is already cached, return it immediately */
    if ((size_t)n < len)
        return cache[n];

    /* Grow the cache if necessary to accommodate index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 8;
        while ((size_t)n >= new_cap)
            new_cap *= 2;            /* double until we can store index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n (inclusive) */
    for (idx_t i = len; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        /* positive contributions */
        if (i >= 3) pos += cache[i - 3] * (seq_t)6;
        if (i >= 4) pos += cache[i - 4] * (seq_t)2;
        if (i >= 5) pos += cache[i - 5];
        if (i >= 7) pos += cache[i - 7] * (seq_t)3;
        /* negative contributions */
        if (i >= 6) neg += cache[i - 6] * (seq_t)3;
        if (i >= 8) neg += cache[i - 8];
        cache[i] = pos - neg; /* pos ≥ neg for all n ≥ 9 */
    }

    len = n + 1;  /* update length to reflect stored values */
    return cache[(size_t)n];
}
