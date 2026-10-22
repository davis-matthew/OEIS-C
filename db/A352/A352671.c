/*
 * A352671  a(n) is the number of nonnegative numbers k < n such that for any
 *          base b >= 2, the sum of digits of n and k in base b are different.
 * https://oeis.org/A352671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of non‑negative integers k < n such that for
 *            every base b ≥ 2 the base‑b digit sums of k and of n are
 *            different
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case for offset 1 */
    if (n == 0) return 0;

    /* Array holding whether a given k is still a candidate.
       Initially all k < n are feasible. */
    bool *valid = (bool *)malloc((size_t)n * sizeof(bool));
    if (!valid) return 0;
    for (size_t i = 0; i < n; ++i) valid[i] = true;

    /* Temporary array for digit sums of all k in the current base. */
    seq_t *sum = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!sum) { free(valid); return 0; }

    /* For every base b from 2 up to n */
    for (idx_t b = 2; b <= n; ++b) {
        /* Digit sum of n in base b */
        seq_t sum_n = 0;
        idx_t tmp = n;
        while (tmp > 0) {
            sum_n += (seq_t)(tmp % b);
            tmp /= b;
        }

        /* Compute digit sums for all k < n in this base */
        sum[0] = 0;
        for (idx_t k = 1; k < n; ++k) {
            idx_t q = k / b;
            idx_t r = k % b;
            sum[k] = sum[q] + (seq_t)r;
        }

        /* Mark ineligible k's */
        for (idx_t k = 0; k < n; ++k) {
            if (valid[k] && sum[k] == sum_n)
                valid[k] = false;
        }
    }

    /* Count the remaining valid k's */
    seq_t result = 0;
    for (idx_t k = 0; k < n; ++k)
        if (valid[k]) result++;

    free(valid);
    free(sum);
    return result;
}
