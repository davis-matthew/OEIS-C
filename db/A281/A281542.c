/*
 * A281542  Expansion of Sum_{i>=1} x^(i^2)/(1 + x^(i^2)) * Product_{j>=1} (1
 *          + x^(j^2)).
 * https://oeis.org/A281542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into
 *            distinct squares
 * Range:     u8 n <= 354, u16 n <= 1520, u32 n <= 7655, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cached DP tables and the maximum index that has been computed. */
static seq_t *dp = NULL;
static seq_t *parts = NULL;
static idx_t max_n_computed = 0;

/* Recompute both tables up to `max`. */
static void recompute(idx_t max)
{
    /* Free previously allocated tables. */
    if (dp)          free(dp);
    if (parts)       free(parts);

    /* Allocate new zeroed tables. */
    dp    = (seq_t*)calloc((size_t)max + 1, sizeof(seq_t));
    parts = (seq_t*)calloc((size_t)max + 1, sizeof(seq_t));
    if (!dp || !parts) {                      /* OOM – caller will receive 0. */
        return;
    }

    /* Base case. */
    dp[0]    = 1;
    parts[0] = 0;

    /* Process each distinct square t = k*k. */
    for (idx_t k = 1; k <= max / k; ++k) {     /* ensures k*k <= max and no overflow */
        idx_t t = k * k;
        for (idx_t s = max; s >= t; --s) {
            seq_t ways = dp[s - t];
            dp[s]    += ways;
            parts[s] += parts[s - t] + ways;   /* new part contributes 1 per partition. */
        }
    }

    max_n_computed = max;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* below offset – undefined. */
    if (n > max_n_computed) recompute(n);
    if (!parts) return 0;                 /* in case of OOM */
    return parts[n];
}
