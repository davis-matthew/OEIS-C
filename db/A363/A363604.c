/*
 * A363604  Expansion of Sum_{k>0} x^(2*k)/(1-x^k)^4.
 * https://oeis.org/A363604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (sigma_3(n) - sigma(n))/6
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 2465, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static caches for divisor sums */
static seq_t *sigma1 = NULL;
static seq_t *sigma3 = NULL;
static idx_t cached_max = 0;

/* Recompute sigma_1 and sigma_3 up to n */
static void recompute_cache(idx_t n)
{
    /* Allocate arrays of size n+1 and zero them */
    sigma1 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    sigma3 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!sigma1 || !sigma3) {
        /* Allocation failed – abort */
        exit(1);
    }

    /* Sieve-like accumulation of divisor sums */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;
        for (idx_t m = d; m <= n; m += d) {
            sigma1[m] += (seq_t)d;
            sigma3[m] += d3;
        }
    }
    cached_max = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* under‑offset behaviour is irrelevant */
    if (n > cached_max) recompute_cache(n);
    seq_t s3 = sigma3[n];
    seq_t s1 = sigma1[n];
    return (s3 - s1) / (seq_t)6;
}
