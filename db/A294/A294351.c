/*
 * A294351  Product of first n terms of the binomial transform of the number
 *          of partitions into distinct parts (A000009).
 * https://oeis.org/A294351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} ∑_{i=0}^{k} binom(k,i)·A000009(i), where
 *            A000009(i) is the number of partitions of i into distinct parts.
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static storage for memoization */
static seq_t *dp       = NULL;   /* A000009[0..max_n]                 */
static seq_t *prod_arr = NULL;   /* a[0..max_n] (product array)       */
static idx_t  max_computed = 0;  /* largest n for which arrays are valid */

/* Allocate new arrays and compute the sequence up to 'need' */
static void ensure_arrays(idx_t need)
{
    if (need <= max_computed)
        return;                /* already computed up to this n */

    /* Allocate fresh arrays big enough for indices 0 … need */
    size_t len = (size_t)need + 1;
    seq_t *new_dp   = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *new_prod = (seq_t*)malloc(len * sizeof(seq_t));

    /* Initialize with zeros */
    memset(new_dp,   0, len * sizeof(seq_t));
    memset(new_prod, 0, len * sizeof(seq_t));

    /* Compute A000009 (partitions into distinct parts) */
    new_dp[0] = (seq_t)1;            /* one partition of 0 */
    for (idx_t part = 1; part <= need; ++part) {
        size_t part_s = (size_t)part;
        for (size_t i = need; i >= part_s; --i) {
            new_dp[i] = new_dp[i] + new_dp[i - part_s];
        }
    }

    /* Compute binomial transforms and cumulative product */
    new_prod[0] = (seq_t)1;          /* a(0) = 1 */
    seq_t prod_val = (seq_t)1;       /* running product */
    for (idx_t k = 1; k <= need; ++k) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;        /* binom(k,0) */
        for (idx_t i = 0; i <= k; ++i) {
            sum = sum + bin * new_dp[(size_t)i];
            if (i < k) {
                seq_t mult = (seq_t)(k - i);
                seq_t divi = (seq_t)(i + 1);
                bin = bin * mult / divi;   /* binom(k,i+1) */
            }
        }
        prod_val = prod_val * sum;     /* multiply into cumulative product */
        new_prod[(size_t)k] = prod_val;
    }

    /* Replace old arrays */
    free(dp);
    free(prod_arr);
    dp = new_dp;
    prod_arr = new_prod;
    max_computed = need;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    ensure_arrays(n);
    return prod_arr[(size_t)n];
}
