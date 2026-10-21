/*
 * A280253  Expansion of Product_{k>=1} (1 + x^p(k)), where p(k) is the number
 *          of partitions of k (A000041).
 * https://oeis.org/A280253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 539, u16 n <= 9796, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280253: Number of partitions of n into distinct partition numbers
   g.f.: Product_{k>=1} (1 + x^p(k)),  where p(k) is the partition function A000041.
   The function computes the coefficient of x^n in that product by dynamic
   programming. For each n it generates all partition numbers p(k) ≤ n and
   performs a 0‑1 knapsack to count subsets summing to n.  The implementation
   keeps a static cache of the partition values so that repeated calls are fast. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of partition numbers p(k).  p_arr[0] = 1 (convention), p_arr[1] = 1, ... */
static seq_t *p_arr = NULL;
static size_t p_size = 0;          /* allocated size of p_arr */
static idx_t  p_next = 1;          /* next k to compute */

/* Ensure that p_arr has space for k (0 <= k < p_size).  Allocate/resize if needed. */
static void ensure_p_size(idx_t k)
{
    if ((size_t)k < p_size)
        return;
    size_t new_size = p_size ? p_size * 2 : 2;
    while ((size_t)k >= new_size)
        new_size *= 2;
    seq_t *tmp = (seq_t *)realloc(p_arr, new_size * sizeof(seq_t));
    if (!tmp) return;          /* allocation failure; caller will handle */
    p_arr = tmp;
    /* initialise new entries to zero; p_arr[0] remains 1 once set */
    for (size_t i = p_size; i < new_size; ++i)
        p_arr[i] = 0;
    p_size = new_size;
    if (p_arr[0] == 0)
        p_arr[0] = (seq_t)1;
}

/* Compute partition number p(k) using the generalized pentagonal number theorem.
   Assumes p_arr[0] is already set to 1 and p_arr[1..k-1] are computed. */
static void compute_p(idx_t k)
{
    if ((size_t)k < p_size && p_arr[(size_t)k] != 0)
        return;               /* already computed */
    ensure_p_size(k);
    seq_t sum = 0;
    for (idx_t m = 1; ; ++m) {
        idx_t g1 = m * (3 * m - 1) / 2;
        if (g1 > k) break;
        seq_t term = p_arr[(size_t)(k - g1)];
        if ((m & 1) == 1) sum += term;   /* odd m gives + */
        else               sum -= term;   /* even m gives - */
        idx_t g2 = m * (3 * m + 1) / 2;
        if (g2 > k) continue;
        term = p_arr[(size_t)(k - g2)];
        if ((m & 1) == 1) sum += term;
        else               sum -= term;
    }
    p_arr[(size_t)k] = sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Handle n == 0 directly */
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP array for subset sum counting */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;                 /* out of memory; return 0 as sentinel */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = (seq_t)1;

    /* Generate partition numbers and process them as items of the knapsack */
    idx_t k = 1;
    while (true) {
        compute_p(k);
        seq_t pk = p_arr[(size_t)k];
        if (pk > (seq_t)n)
            break;                    /* no more useful items */
        size_t w = (size_t)pk;        /* weight fits into n */
        for (size_t i = sz - 1; i >= w; --i) {
            dp[i] += dp[i - w];
            if (i == w) break;       /* avoid unsigned underflow */
        }
        ++k;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
