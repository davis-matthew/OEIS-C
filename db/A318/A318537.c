/*
 * A318537  Irregular triangle read by rows: T(n,m) is the number of n X m
 *          (0,1)-matrices with pairwise distinct nonzero columns and pairwise
 *          distinct nonzero rows, n >= 0, m = 0..2^n-1.
 * https://oeis.org/A318537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row, col) where * row = floor_log2_uint64(n + 1) * col
 *            = n - ( (1ULL << row) - 1 ) * T(n, m) = m! * Σ_{i=0..n}
 *            Stirling1(n+1,i+1) * binomial(2^i-1, m)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 24, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, factorial, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the required signed Stirling numbers only up to
 * the needed order (row+1) and uses the provided helper functions
 * floor_log2_uint64, binomial, and factorial.                             */

#include <stdint.h>
#include <stdbool.h>
/* helpers from the utils directory */
#include "../utils/floor_log2_uint64.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"

static int64_t stirling_table[64][64] = {0};

static void compute_stirling(int64_t up_to)
{
    /* Ensure table contains signed Stirling numbers of the first kind
     * s(k, j) for 0 <= k <= up_to and 0 <= j <= k.  The recurrence is:
     *   s(0,0) = 1
     *   s(k,0) = 0   (k > 0)
     *   s(k,k) = 1   (k > 0)
     *   s(k,j) = s(k-1,j-1) - (k-1) * s(k-1,j)   (1 <= j < k)
     */
    if (up_to < 0) return;
    stirling_table[0][0] = 1;
    for (int64_t k = 1; k <= up_to; ++k) {
        stirling_table[k][0] = 0;
        for (int64_t j = 1; j < k; ++j) {
            stirling_table[k][j] = stirling_table[k-1][j-1] - (k-1) * stirling_table[k-1][j];
        }
        stirling_table[k][k] = 1;
    }
}

seq_t a(idx_t n)
{
    /* Determine triangle position (row, col) from flattened index n */
    idx_t row = floor_log2_uint64(n + 1);
    idx_t row_start = ((idx_t)1 << row) - 1;
    idx_t col = n - row_start;               /* 0 <= col < 2^row */

    /* Prepare Stirling numbers up to order row+1 */
    if ((int)row + 1 > 63) { /* safety: n too large for our table */
        return 0;
    }
    compute_stirling((int64_t)row + 1);

    /* Accumulate contributions in positive and negative parts separately */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 0; i <= row; ++i) {
        int64_t s_val = stirling_table[(int64_t)row + 1][i + 1];
        if (s_val == 0) continue;

        idx_t pow_two_i_minus1 = ((idx_t)1 << i) - 1;
        seq_t bin = binomial(pow_two_i_minus1, col);
        if (bin == 0) continue;

        /* Convert the absolute Stirling value to seq_t */
        seq_t abs_s = (seq_t)((s_val >= 0) ? s_val : -s_val);
        seq_t prod = bin * abs_s;

        if (s_val < 0)
            sum_neg += prod;
        else
            sum_pos += prod;
    }

    /* Difference of the two parts (non-negative for all inputs) */
    seq_t diff = (sum_pos >= sum_neg) ? (sum_pos - sum_neg) : 0;

    /* Multiply by col! */
    seq_t fact = factorial(col);
    return fact * diff;
}
