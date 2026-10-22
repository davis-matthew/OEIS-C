/*
 * A306646  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of g.f. (k+1-x^k)/(1-x^k-x^(k+1)).
 * https://oeis.org/A306646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value read by antidiagonals from the square array *
 *            A(n,k) with n ≥ 0, k ≥ 1. * For n = 0: A(0,k) = k + 1. * For n >
 *            0: A(n,k) = n * Σ_{j=1..⌊n/k⌋} binomial(j, n – k*j) / j. * The
 *            sequence a(n) is the list of A(n,k) enumerated by increasing *
 *            antidiagonal index (n+k–1). For example: * a(0)...
 * Range:     u8 n <= 89, u16 n <= 323, u32 n <= 1174, u64 n <= 4463,
 *            u128 n <= 9869, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static idx_t isqrt_u64(idx_t x)
{
    /* integer square‑root: floor(sqrt(x)) */
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            if (x == 0) return 0;
            lo = mid + 1;
            continue;
        }
        if (mid > x / mid) {
            hi = mid - 1;
        } else {
            if ((mid + 1) > x / (mid + 1)) return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 handled together with (i==0) below. */

    /* Find diagonal index s (starting from 1) such that
     *   sum_{t=1}^{s-1} t  <=  n   <  sum_{t=1}^{s} t
     * i.e. prevTri = (s-1)*s/2  <= n < s*(s+1)/2.
     */
    idx_t mval = 8u * n + 1u;                 /* 8*n + 1 is always odd */
    idx_t r    = isqrt_u64(mval);             /* floor(sqrt(8*n+1)) */
    idx_t s    = (r - 1u) / 2u + 1u;          /* diagonal number */
    idx_t prevTri = (s - 1u) * s / 2u;        /* terms before this diagonal */
    idx_t offset   = n - prevTri;             /* 0‑based position within diagonal */

    /* Convert offset to (row, column) */
    idx_t row = offset;           /* n-coordinate */
    idx_t col = s - offset;       /* k-coordinate (col ≥ 1) */

    /* A(0,k) = k+1 */
    if (row == 0u)
        return (seq_t)(col + 1u);

    /* A(n,k) = 0 when n < k  (below the diagonal) */
    if (row < col)
        return (seq_t)0u;

    /* Compute the sum:  A(row, col) = row * Σ_{j=1..floor(row/col)} binomial(j, row-col*j)/j. */
    seq_t result = 0u;
    idx_t maxJ   = row / col;                 /* floor(row/col) */

    for (idx_t j = 1u; j <= maxJ; ++j) {
        idx_t rest = row - col * j;           /* n – k*j  */
        if (rest > j) continue;              /* binomial(j,rest) = 0 for rest > j */
        seq_t bin = binom(j, rest);           /* uses utilities */
        if (bin == 0u) continue;
        seq_t term = ((seq_t)row * bin) / j;  /* division is exact by definition */
        result += term;
    }

    return result;
}
