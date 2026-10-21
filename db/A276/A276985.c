/*
 * A276985  Triangle read by rows: T(n,k) = number of k-dimensional elements
 *          in an n-dimensional cross-polytope, n>=1, 0<=k<n.
 * https://oeis.org/A276985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattening of the triangle T(r,k)=2^(k+1) *
 *            binomial(r,k+1) for r≥1, 0≤k<r. For a given index n (offset 1)
 *            we find the row r with r(r−1)/2 < n ≤ r(r+1)/2 and the column k
 *            = n − r(r−1)/2 − 1. Then a(n)=2^(k+1)·C(r,k+1)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* sequence is defined only for n≥1 */

    /* locate the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;      /* Σ_{i=1}^{r} i */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* previous cumulative sum before the current row */
    idx_t prev_cumulative = cumulative - r;

    /* column index k (0‑based) within the row */
    idx_t k = n - prev_cumulative - 1;

    /* compute T(r,k) = 2^(k+1) * binomial(r,k+1) */
    seq_t twop = pow2(k + 1);       /* 2^(k+1) */
    seq_t bino = binomial(r, k + 1);/* C(r,k+1) */
    return twop * bino;
}
