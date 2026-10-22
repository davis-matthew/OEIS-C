/*
 * A368236  Expansion of e.g.f. 1/(exp(-x) - 2*x).
 * https://oeis.org/A368236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2*n*a(n-1) + Sum_{k=1..n} (-1)^{k-1} *
 *            binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max   = 0;

    /* initialise cache lazily */
    if (cache == NULL) {
        cap   = 16;                      /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)1;             /* a(0) */
        max = 0;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max + 1; i <= n; ++i) {
        /* 2*i*a(i-1) part */
        seq_t term = ((seq_t)2 * (seq_t)i) * cache[i - 1];

        /* alternating binomial sum */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        seq_t C = 1; /* C(i,0) = 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* compute C(i,k) multiplicatively */
            C = (C * (seq_t)(i - k + 1)) / (seq_t)k;

            seq_t prod = C * cache[i - k];
            if (k & 1) /* odd k -> positive */
                sum_pos += prod;
            else       /* even k -> negative */
                sum_neg += prod;
        }

        term += (sum_pos - sum_neg);
        cache[i] = term;
    }

    max = n;
    return cache[n];
}
