/*
 * A336610  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(-sqrt(x) *
 *          BesselI(1,2*sqrt(x))).
 * https://oeis.org/A336610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -n * Sum_{k=0}^{n-1} binomial(n-1,k)^2 * a(k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* External types supplied by the build
   seq_t  : signed integer type (8/16/32/64/128 bits or arbitrary‑precision)
   idx_t  : unsigned integer type (uint64_t) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* storage for computed values */
    static size_t cache_cap = 0;         /* allocated capacity */
    static size_t next_computed = 0;     /* index of first value yet to be computed */

    /* Ensure the cache can hold indices up to n */
    if (n >= cache_cap) {
        cache = (seq_t *)realloc((void *)cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* Initialize the first value a(0) */
    if (next_computed == 0) {
        cache[0] = (seq_t)1;
        next_computed = 1;
    }

    /* Compute missing values up to n */
    while (next_computed <= n) {
        idx_t i = next_computed;
        seq_t sum = 0;
        seq_t binom = (seq_t)1;  /* binomial(i-1, 0) */

        for (idx_t k = 0; k < i; ++k) {
            /* term = binom^2 * a(k) */
            seq_t term = binom * binom;
            term = term * cache[k];
            sum += term;

            /* update binom for next k: binom(i-1,k+1) */
            if (k < i - 1) {
                binom = (binom * (seq_t)(i - 1 - k)) / (seq_t)(k + 1);
            }
        }

        /* a(i) = -i * sum */
        cache[i] = -(seq_t)i * sum;
        ++next_computed;
    }

    return cache[n];
}
