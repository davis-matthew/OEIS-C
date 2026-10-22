/*
 * A308322  A(n,k) = Sum_{i_1=0..n} Sum_{i_2=0..n} ... Sum_{i_k=0..n}
 *          (-1)^(i_1 + i_2 + ... + i_k) * multinomial(i_1 + i_2 + ... + i_k;
 *          i_1, i_2, ..., i_k), square array A(n,k) read by antidiagonals,
 *          for n >= 0, k >= 0.
 * https://oeis.org/A308322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A(n,k) = Sum_{i_1=0..n, …, i_k=0..n} (-1)^{i_1+…+i_k} * *
 *            multinomial(i_1+…+i_k; i_1,…,i_k) * Antidiagonal type: a(m) =
 *            A(n,k) where n+k = s and * s(s+1)/2 ≤ m < (s+1)(s+2)/2. * Then n
 *            = m - s(s+1)/2, k = s - n. * Returns the term a(m) for any
 *            non‑negative index m
 * Range:     i8 n <= 22, i16 n <= 29, i32 n <= 47, i64 n <= 69, i128 n <= 123,
 *            big any n
 * Helpers:   alloc_seq_array, binomial, free_seq_array, int_sqrt, sum_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, free */
#include <stdbool.h>  /* bool */

/* Utility headers provided by the build.  Their functions are
 * declared as extern and available to the compiler. */
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"   /* void free_seq_array(seq_t *) */
#include "../utils/binomial.h"        /* seq_t binomial(idx_t, idx_t) */
#include "../utils/int_sqrt.h"        /* idx_t int_sqrt(idx_t) */
#include "../utils/sum_array.h"

/* Compute A(n,k) where 0 ≤ n,k.  Uses integer DP that avoids fractions. */
static seq_t compute_A(idx_t n, idx_t k)
{
    if (k == 0)           /* special case */
        return (seq_t)1;

    size_t max_sum = (size_t)k * (size_t)n;   /* largest possible total */
    size_t curr_len = 1;                       /* only dp[0] exists yet */
    seq_t *dp = alloc_seq_array(max_sum + 1);
    for (size_t i = 0; i <= max_sum; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;                          /* start with sum 0 */

    for (idx_t step = 0; step < k; ++step) {
        size_t prev_len = curr_len;
        size_t new_len  = (size_t)(step + 1) * (size_t)n + 1;
        seq_t *next = alloc_seq_array(max_sum + 1);
        for (size_t i = 0; i < new_len; ++i) next[i] = 0;

        /* Convolve with a new variable ranging from 0..n. */
        for (size_t sum_prev = 0; sum_prev < prev_len; ++sum_prev) {
            seq_t base_val = dp[sum_prev];
            if (base_val == 0) continue;

            for (idx_t t = 0; t <= n; ++t) {
                size_t new_sum = sum_prev + (size_t)t;
                if (new_sum >= new_len) break;      /* out of range */

                seq_t comb = binomial(new_sum, t);
                seq_t prod = base_val * comb;      /* binomial * base */
                if (t & 1) prod = -prod;          /* sign factor */

                next[new_sum] += prod;            /* accumulate */
            }
        }

        free_seq_array(dp);
        dp = next;
        curr_len = new_len;
    }

    /* Sum all dp entries to get A(n,k). */
    seq_t result = sum_array(dp, curr_len);
    free_seq_array(dp);
    return result;
}

/* Maps a linear index m to the antidiagonal coordinates (n,k). */
static void antidiagonal_coords(idx_t m, idx_t *n_out, idx_t *k_out)
{
    if (m == 0) {
        *n_out = 0;
        *k_out = 0;
        return;
    }

    /* Solve s(s+1)/2 ≤ m < (s+1)(s+2)/2. */
    idx_t t = 8 * m + 1;
    idx_t s = (int_sqrt(t) - 1) / 2;     /* floor of the quadratic solution */

    /* Correct possible rounding errors. */
    while (((idx_t)(s + 1) * (s + 2)) / 2 <= m) ++s;
    while (((idx_t)s * (s + 1)) / 2 > m) --s;

    idx_t before = (idx_t)s * (s + 1) / 2;
    *n_out = m - before;          /* 0 ≤ n ≤ s */
    *k_out = s - *n_out;          /* 0 ≤ k ≤ s */
}

/* Public entry point required by the specification. */
seq_t a(idx_t m)
{
    idx_t n, k;
    antidiagonal_coords(m, &n, &k);
    return compute_A(n, k);
}
