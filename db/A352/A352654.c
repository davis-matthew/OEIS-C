/*
 * A352654  a(0) = 0, a(n) = [x^n] P(n-1,(1 + x)/(1 - x)) for n >= 1, where
 *          P(n,x) denotes the n-th Legendre polynomial.
 * https://oeis.org/A352654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n-1}
 *            binomial(n-1,k)*binomial(n-1,k-1)*binomial(n+k-1,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(0) = 0, and the sum is empty for n = 0 or 1, giving a(1) = 0.) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0. */
    if (n == 0) return 0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size of cache  */
    static idx_t max_n     = 0;   /* highest n whose value is known */

    /* Allocate cache if this is the first call. */
    if (!cache) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();        /* out of memory */
        cache[0] = 0;               /* a(0) = 0 */
        max_n = 0;
    }

    /* Grow cache if necessary. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* Compute values only up to n if they are not yet known. */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* Initial term for k = 1: C1 = binom(i-1,1) = i-1. */
            seq_t C1 = (seq_t)(i - 1);      /* binom(i-1, k)  (k=1) */
            seq_t C2 = 1;                   /* binom(i-1, k-1) */
            seq_t C3 = (seq_t)i;            /* binom(i+k-1,k) for k=1 gives binom(i,1) = i */
            sum += C1 * C2 * C3;

            /* Increment k from 2 up to i-1. */
            for (idx_t k = 2; k <= i - 1; ++k) {
                seq_t k_seq   = (seq_t)k;
                seq_t factor1 = (seq_t)(i - k);        /* n - k   */
                seq_t factor2 = (seq_t)(i + k - 1);    /* n + k - 1 */

                /* Compute binom(i-1, k) from previous value. */
                seq_t g1 = gcd(C1, k_seq);
                seq_t C1_div = C1 / g1;
                seq_t k_div  = k_seq / g1;

                seq_t g2 = gcd(factor1, k_div);
                seq_t f1_red   = factor1 / g2;
                seq_t k_divred = k_div  / g2;

                seq_t C1_next = (C1_div * f1_red) / k_divred;

                /* C2 for this k is the previous C1. */
                seq_t C2_next = C1;

                /* Compute binom(i+k-1, k) from previous value. */
                seq_t g3 = gcd(C3, k_seq);
                seq_t C3_div = C3 / g3;
                seq_t k_div3  = k_seq / g3;

                seq_t g4 = gcd(factor2, k_div3);
                seq_t f2_red   = factor2 / g4;
                seq_t k_div3red = k_div3 / g4;

                seq_t C3_next = (C3_div * f2_red) / k_div3red;

                /* Add the term for this k. */
                seq_t term = C1_next * C2_next * C3_next;
                sum += term;

                /* Advance to next k. */
                C1 = C1_next;
                C3 = C3_next;
            }
            cache[i] = sum;
        }
        max_n = n;
    }

    return cache[n];
}
