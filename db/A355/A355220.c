/*
 * A355220  a(n) = Sum_{k>=1} (4*k - 1)^n / 2^k.
 * https://oeis.org/A355220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; 
   a(n)=3^n + Σ_{k=1..n} binomial(n,k)*4^k*a(n-k) */

#include <stdlib.h>

/* Function to return the n-th term of sequence A355220
 * (the function must have external linkage). */
seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *a_cache = NULL;
    static size_t a_cap   = 0;

    /* cache of powers of 3 to avoid recomputation */
    static seq_t *pow3_cache = NULL;
    static size_t pow3_cap   = 0;

    /* first call: initialize cache for a(0) and 3^0 */
    if (a_cap == 0) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cap);
        a_cache[0] = (seq_t)1;            /* a(0) = 1 */

        pow3_cap = 1;
        pow3_cache = (seq_t *)malloc(sizeof(seq_t) * pow3_cap);
        pow3_cache[0] = (seq_t)1;          /* 3^0 = 1 */
    }

    /* expand caches if requested index is larger than current capacity */
    if ((size_t)n >= a_cap) {
        size_t new_cap = (size_t)n + 1;

        /* grow the cache arrays */
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        pow3_cache = (seq_t *)realloc(pow3_cache, sizeof(seq_t) * new_cap);

        /* compute new powers of 3 if needed */
        for (size_t i = pow3_cap; i < new_cap; ++i) {
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
        }

        /* compute new sequence terms */
        for (size_t i = a_cap; i < new_cap; ++i) {
            /* start with 3^i */
            seq_t sum = pow3_cache[i];

            /* binomial coefficient C(i, k), initialized for k=0 */
            seq_t binom = (seq_t)1;
            /* current power of 4, starting with 4^1 */
            seq_t pow4 = (seq_t)4;

            /* sum over k = 1 .. i */
            for (size_t k = 1; k <= i; ++k) {
                /* update binomial coefficient: C(i, k) = C(i, k-1) * (i-k+1)/k */
                binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
                /* accumulate term: binom * 4^k * a(i - k) */
                seq_t term = binom * pow4 * a_cache[i - k];
                sum += term;
                /* next power of 4 */
                pow4 *= (seq_t)4;
            }

            a_cache[i] = sum;
        }

        a_cap   = new_cap;
        pow3_cap = new_cap;
    }

    return a_cache[(size_t)n];
}
