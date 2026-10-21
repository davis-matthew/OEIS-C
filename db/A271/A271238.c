/*
 * A271238  Triangle of numbers where T(n,k) is the number of k-dimensional
 *          faces on a completely truncated n-dimensional simplex, 0 <= k <=
 *          n.
 * https://oeis.org/A271238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A271238 – Triangle of numbers where T(n,k) = number of k-dimensional
 * faces on a completely truncated n-simplex, 0 ≤ k ≤ n.
 *
 * Formula used:
 *   • r = row index (0‑based) such that (r+1)(r+2)/2 > n.
 *   • c = column index within that row, 0 ≤ c ≤ r.
 *   • T(r,0)          = r*(r+1)/2                      (r ≥ 2)
 *   • T(r,r)          = 1
 *   • T(0,0)          = 1
 *   • T(1,0)          = 2,  T(1,1) = 1
 *   • T(r,1)          = binom(r,1) + (r+2)*binom(r,2) - binom(r+1,2)   (r ≥ 2 && c==1)
 *   • T(r,c) for 1 < c < r = binom(r,c) + (r+2)*binom(r,c+1)            (r ≥ 2)
 *
 * This expression is derived from the generating‑function formula on OEIS:
 *   ((x+1)^r-1)*(x+r+2)/x - r - binom(r+1,2)*(x+1)
 * and the coefficients are extracted explicitly.
 *
 * The implementation uses the provided `binomial(idx_t n, idx_t k)` helper.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find the row r containing the index n.  The cumulative length of
       rows 0..r is (r+1)(r+2)/2. We want the smallest r with that sum > n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Compute the index of the element within row r. */
    idx_t prev = (r * (r + 1)) / 2;   /* length of rows 0..r-1 */
    idx_t c = n - prev;               /* 0 ≤ c ≤ r */

    /* Special handling for the first two rows. */
    if (r == 0)                 /* row 0: 1 */
        return (seq_t)1;

    if (r == 1) {               /* row 1: 2, 1 */
        return (c == 0) ? (seq_t)2 : (seq_t)1;
    }

    /* r >= 2 */
    if (c == 0) {               /* first element of the row */
        return (seq_t)r * ((seq_t)r + 1) / 2;
    }

    if (c == r) {               /* last element of the row */
        return (seq_t)1;
    }

    if (c == 1) {               /* second element – special case */
        seq_t part1 = binomial(r, 1);
        seq_t part2 = (seq_t)(r + 2) * binomial(r, 2);
        seq_t part3 = binomial(r + 1, 2);
        return part1 + part2 - part3;
    }

    /* 1 < c < r */
    seq_t part1 = binomial(r, c);
    seq_t part2 = (seq_t)(r + 2) * binomial(r, c + 1);
    return part1 + part2;
}
