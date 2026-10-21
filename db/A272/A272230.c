/*
 * A272230  E.g.f.: 2*exp(x)/(exp(2*x)+1+2*x).
 * https://oeis.org/A272230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 - 2*n*a(n-1) - Sum_{k=0..n-2} binom(n,k)*2^{n-k-1}*a(k)
 *            a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp) */
#include "../utils/binomial.h" /* seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  sz     = 0;   /* number of entries stored */

    /* initialise cache for first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        sz = 0;
    }

    /* expand cache capacity if needed */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* set base value a(0)=1 */
    if (sz == 0) {
        cache[0] = (seq_t)1;
        sz = 1;
    }

    /* if n==0, return */
    if (n == 0)
        return cache[0];

    /* compute a(1) if not yet computed */
    if (sz == 1) {
        cache[1] = (seq_t)1 - ((seq_t)2 * (seq_t)1) * cache[0];
        sz = 2;
        if (n == 1)
            return cache[1];
    }

    /* compute terms from sz up to n */
    for (idx_t i = (idx_t)sz; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;  /* binom(i,0) */

        /* sum over k = 0 .. i-2  (empty when i<2) */
        for (idx_t k = 0; k <= i - 2; ++k) {
            seq_t term = bin * pow2(i - k - 1);
            term *= cache[(size_t)k];
            sum += term;

            /* update bin = binom(i,k+1) */
            if (k < i - 2) {
                bin = (bin * (seq_t)(i - k)) / (seq_t)(k + 1);
            }
        }

        seq_t val = (seq_t)1 - ((seq_t)i * (seq_t)2) * cache[(size_t)i - 1];
        val -= sum;
        cache[(size_t)i] = val;
    }
    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
