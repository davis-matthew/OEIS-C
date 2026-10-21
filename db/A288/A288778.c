/*
 * A288778  Triangle read by rows (1<=k<=n): T(n,k) = (n-k+1)*k! - (k-1)!
 * https://oeis.org/A288778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 ≥ n, c =
 *            n − r(r−1)/2, T(r,c) = (r − c + 1)·c! − (c − 1)!
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  SPDX-License-Identifier: MIT  */
#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find triangle row r and position k (column).  */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* cum = r(r+1)/2,  previous sum is cum - r = r(r-1)/2 */
    idx_t prev = cum - r;          /* sum_{i=1}^{r-1} i */
    idx_t k = n - prev;            /* 1 ≤ k ≤ r */

    /* Compute the triangle entry: (r-k+1)*k! - (k-1)! */
    seq_t fact_k = factorial(k);
    seq_t fact_k_minus1 = factorial(k - 1);

    seq_t result = (seq_t)(r - k + 1) * fact_k - fact_k_minus1;
    return result;
}
