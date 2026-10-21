/*
 * A241091  Number of partitions p of n into distinct parts such that max(p)
 *          <= 1 + 2*(number of parts of p).
 * https://oeis.org/A241091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into distinct parts such that
 *            max(part) <= 1 + 2*(number of parts)
 * Range:     u8 n <= 44, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of helper */
static void build_sequence(idx_t max_n, seq_t *a);

/* Computes the sequence up to index max_n and stores the
   results in the array a[0..max_n].  */
static void build_sequence(idx_t max_n, seq_t *a)
{
    /* Find maximal number of parts needed: minimal sum of k distinct
       positive integers is k*(k+1)/2 */
    idx_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= max_n) ++kmax;

    idx_t Mmax = 1 + 2 * kmax;           /* largest part needed */

    size_t stride = (size_t)(max_n + 1);
    /* dp[k][sum] flattened as dp[k*stride + sum] */
    seq_t *dp = (seq_t*)calloc((size_t)(kmax + 1) * stride, sizeof(seq_t));
    if (!dp) abort();
    dp[0 * stride + 0] = 1;              /* empty set */

    /* Count subsets of size k from numbers up to v.
       After processing v = 1 + 2*k, dp[k] contains all subsets
       with maximum <= v, so we snapshot their contributions. */
    for (idx_t v = 1; v <= Mmax; ++v) {
        idx_t k_limit = v < kmax ? v : kmax;
        for (idx_t k = k_limit; k >= 1; --k) {
            size_t k_index = (size_t)k * stride;
            size_t prev_index = (size_t)(k - 1) * stride;
            for (idx_t sum = max_n; sum >= v; --sum) {
                dp[k_index + sum] += dp[prev_index + sum - v];
            }
        }
        /* snapshot when v equals 1+2*k for some k */
        if (v >= 3 && ((v - 1) & 1) == 0) {
            idx_t k = (v - 1) >> 1;
            if (k <= kmax) {
                size_t k_index = (size_t)k * stride;
                for (idx_t sum = 0; sum <= max_n; ++sum) {
                    a[sum] += dp[k_index + sum];
                }
            }
        }
    }

    free(dp);
}

/* Public function required by the interface */
seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;
    static idx_t a_size = 0;

    if (n + 1 > a_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_a = (seq_t*)calloc(new_size, sizeof(seq_t));
        if (!new_a) abort();
        build_sequence(n, new_a);
        if (a_vals) free(a_vals);
        a_vals = new_a;
        a_size = (idx_t)new_size;
    }
    return a_vals[n];
}
