/*
 * A373074  Number of partitions of n such that (smallest part) > 3*(number of
 *          parts).
 * https://oeis.org/A373074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} P(n−k·(3k+1), ≤k) where P(m, ≤k) is the number of
 *            partitions of m into parts not exceeding k (equivalently,
 *            partitions of m into at most k parts). The empty partition of 0
 *            is counted once, giving a(0)=1
 * Range:     u8 n <= 69, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No type definitions; seq_t and idx_t are supplied by the build system. */
#include <stdlib.h>

/* Helper: compute the floor of the square root of a 64‑bit integer
   using only integer arithmetic.  This function is not strictly
   required for the problem (n is small), but it is kept self‑contained. */
static idx_t floor_isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The empty partition of 0 is the only valid partition of n=0. */
    if (n == 0) {
        return 1;
    }

    /* Determine the largest integer k such that k*(3k+1) ≤ n. */
    idx_t k_max = 0;
    while ((k_max + 1) * (3 * (k_max + 1) + 1) <= n) {
        ++k_max;
    }

    /* Allocate a table for partitions of m into parts ≤ k.
       table[m][k] is stored at table[(m)*(k_max+1) + k]. */
    idx_t rows = n + 1;          /* m ranges from 0 … n */
    idx_t cols = k_max + 1;      /* k ranges from 0 … k_max */
    seq_t *table = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!table) return 0;        /* fallback on allocation failure */

    /* Base case: P(0, ≤k) = 1 for all k.  calloc already zeroed
       the array, so we only need to set the zero row. */
    for (idx_t k = 0; k < cols; ++k) {
        table[0 * cols + k] = 1;
    }

    /* Compute the table using the recurrence
         P(m, ≤k) = P(m, ≤k−1) + P(m−k, ≤k)  (if m ≥ k)
    */
    for (idx_t k = 1; k < cols; ++k) {
        for (idx_t m = 1; m <= n; ++m) {
            seq_t val = table[m * cols + (k - 1)];
            if (m >= k) {
                val += table[(m - k) * cols + k];
            }
            table[m * cols + k] = val;
        }
    }

    /* Sum over all admissible numbers of parts k. */
    seq_t total = 0;
    for (idx_t k = 1; k <= k_max; ++k) {
        idx_t m_min = 3 * k + 1;            /* smallest part must exceed 3k */
        idx_t product = k * m_min;          /* k * m_min ≤ n by construction */
        if (product > n) continue;          /* safety check */
        idx_t rem = n - product;            /* remainder after subtracting k*m_min */
        total += table[rem * cols + k];     /* P(rem, ≤k) */
    }

    free(table);
    return total;
}
