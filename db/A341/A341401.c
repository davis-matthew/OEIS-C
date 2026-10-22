/*
 * A341401  Number of nonnegative solutions to (x_1)^2 + (x_2)^2 + ... +
 *          (x_6)^2 <= n.
 * https://oeis.org/A341401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer 6‑tuples * (x1,…,x6) such
 *            that x1² + x2² + … + x6² ≤ n. The function * evaluates this
 *            quantity for any 0 ≤ n
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.
 * Returns the largest k such that k² ≤ n. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = 4294967295ULL;          /* √(2⁶⁴−1) < 2³² */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Count number of nonnegative integer solutions to
 * x1² + x2² + … + x6² ≤ n. */
seq_t a(idx_t n)
{
    /* Maximum square root needed. */
    idx_t max_sqrt = isqrt_u64(n);

    /* Array of squares ≤ n. */
    size_t sq_cnt = (size_t)(max_sqrt + 1);
    idx_t *squares = (idx_t *)malloc(sq_cnt * sizeof(idx_t));
    if (!squares) return (seq_t)0;          /* allocation failure */
    for (idx_t m = 0; m <= max_sqrt; ++m) {
        squares[(size_t)m] = m * m;
    }

    /* DP arrays: counts of tuples giving exact sum s. */
    size_t size = (size_t)(n + (idx_t)1);
    seq_t *dp_prev = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp_prev || !dp_curr) {
        free(squares);
        free(dp_prev);
        free(dp_curr);
        return (seq_t)0;                    /* allocation failure */
    }

    /* Initially, with no variables, only sum 0 is possible. */
    dp_prev[0] = (seq_t)1;

    /* Convolve the polynomial 1 + x^1 + x^4 + … for six variables. */
    for (idx_t var = 1; var <= 6; ++var) {
        /* Zero dp_curr for the next iteration. */
        for (size_t i = 0; i < size; ++i) dp_curr[i] = (seq_t)0;

        /* Convolution step. */
        for (size_t si = 0; si < sq_cnt; ++si) {
            idx_t sq = squares[si];
            if (sq > n) break;               /* should not happen */
            for (idx_t t = sq; t <= n; ++t) {
                dp_curr[(size_t)t] += dp_prev[(size_t)(t - sq)];
            }
        }

        /* Swap pointers for next variable. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Sum counts for all sums ≤ n. */
    seq_t total = (seq_t)0;
    for (size_t s = 0; s < size; ++s) total += dp_prev[s];

    /* Clean up. */
    free(squares);
    free(dp_prev);
    free(dp_curr);

    return total;
}
