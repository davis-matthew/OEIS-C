/*
 * A257635  Triangle with n-th row polynomial equal to Product_{k = 1..n} (x +
 *          n + k).
 * https://oeis.org/A257635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (row-major) entry of the triangle T * where T(r,k) =
 *            coefficient of x^k in ∏_{j=r+1}^{2r} (x + j), * 0 ≤ k ≤ r. The
 *            triangle is * * [0] 1; * [1] 2, 1; * [2] 12, 7, 1; * ... * * The
 *            sequence a(n) lists the triangle entries in order: * a(0)=1,
 *            a(1)=2, a(2)=1, a(3)=12, a(4)=7, a(5)=1, .....
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#pragma once

/* Compute the row r such that the n-th element lies in that row.
 * The cumulative number of terms up to row r is r*(r+1)/2.
 * We find the largest r with r*(r+1)/2 ≤ n. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;  /* high is an upper bound for r */
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        __int128 cum = (__int128)mid * (mid + 1) / 2;  /* avoid overflow */
        if (cum <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;  /* largest r with r*(r+1)/2 ≤ n */
}

seq_t
a(idx_t n)
{
    /* Determine row r and position k within that row. */
    idx_t r = find_row(n);
    idx_t cumulative_before = r * (r + 1) / 2;
    idx_t k = n - cumulative_before;  /* 0 ≤ k ≤ r */

    /* Allocate coefficient array of size k+1. */
    seq_t *coeff = (seq_t *)calloc((size_t)(k + 1), sizeof(seq_t));
    if (!coeff) return 0;  /* allocation failure (unlikely) */

    coeff[0] = 1;  /* start with polynomial = 1 */

    /* Multiply successively by (x + j) for j = r+1 … 2r. */
    for (idx_t j = r + 1; j <= 2 * r; ++j)
    {
        seq_t sj = (seq_t)j;
        /* Update coefficients in reverse order to use previous values. */
        for (idx_t d = k; d > 0; --d)
            coeff[d] = coeff[d] * sj + coeff[d - 1];
        coeff[0] *= sj;
    }

    seq_t result = coeff[k];
    free(coeff);
    return result;
}
