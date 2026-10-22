/*
 * A320589  Expansion of (1/(1 + x)) * Sum_{k>=1} k*x^k/(x^k + (1 + x)^k).
 * https://oeis.org/A320589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} (-1)^(n-k) * binomial(n,k) * A000593(k) where
 *            A000593(k) is the sum of odd divisors of k
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 28, i64 n <= 59, i128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static buffers for A000593 values and memoised a(n) results. */
static seq_t *odd_div_sum = NULL;   /* odd_div_sum[k] = A000593(k) */
static size_t odd_div_sum_cap = 0;  /* capacity of odd_div_sum       */
static seq_t *a_cache = NULL;       /* memoised a(n) values          */
static size_t a_cache_cap = 0;      /* capacity of a_cache           */

/* Ensure that odd_div_sum[0..n] and a_cache[0..n] exist. */
static void ensure_buffers(idx_t n)
{
    size_t N = (size_t)n;
    if (N >= odd_div_sum_cap) {
        /* Reallocate and recompute odd_div_sum. */
        free(odd_div_sum);
        odd_div_sum = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!odd_div_sum) abort();
        for (size_t d = 1; d <= N; d += 2) {        /* only odd d */
            for (size_t m = d; m <= N; m += d) {
                odd_div_sum[m] += (seq_t)d;          /* sum odd divisors */
            }
        }
        odd_div_sum_cap = N + 1;
    }

    if (N >= a_cache_cap) {
        free(a_cache);
        a_cache = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!a_cache) abort();
        a_cache_cap = N + 1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* value below offset is undefined */

    ensure_buffers(n);
    size_t ui = (size_t)n;

    /* Return cached value if already computed (except for n == 1). */
    if (n != 1) {
        seq_t cached = a_cache[ui];
        if (cached != 0) return cached;
    }

    seq_t result = 0;
    seq_t binom = (seq_t)n;                /* binomial(n,1) */
    int sign = ((int)((n - 1) & 1)) ? -1 : 1; /* (-1)^(n-1) */

    for (size_t k = 1; k <= ui; ++k) {
        seq_t term = binom * odd_div_sum[k];
        if (sign < 0) term = -term;
        result += term;
        if (k < ui) {
            /* Update binomial coefficient to binomial(n, k+1).  The division
             * is exact, so integer truncation does not affect the result. */
            binom = (seq_t)((seq_t)binom * (seq_t)(n - k) / (seq_t)(k + 1));
            sign = -sign;                 /* alternate sign */
        }
    }

    a_cache[ui] = result;
    return result;
}
