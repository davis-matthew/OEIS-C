/*
 * A259569  Triangle T(n,k) read by rows, where T(n,k) is the number of
 *          k-dimensional faces of the polytope that is the convex hull of all
 *          permutations of the list (0,1,...,1,2), where there are n - 1
 *          ones, for n > 0. T(0,0) is 1.
 * https://oeis.org/A259569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry (row–major) of the triangle * T(n,k) where
 *            * T(n,n) = 1, * T(n,n-1) = 2^(n+1)-2, * T(n,0) = n*(n+1), *
 *            T(n,k) = binomial(n+1,k+2)*(4*2^k-2) for 0 <= k < n
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k such that n = r*(r+1)/2 + k, 0 <= k <= r. */
    if (n == 0) return 1;          /* T(0,0) = 1 */

    /* Approximate the row using the quadratic formula, then adjust. */
    double t = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((t - 1.0) / 2.0);

    /* Correct any rounding errors caused by double arithmetic. */
    idx_t start = r * (r + 1) / 2;
    while (start > n) {
        --r;
        start = r * (r + 1) / 2;
    }
    while (start + r + 1 <= n) {
        ++r;
        start = r * (r + 1) / 2;
    }

    idx_t k = n - start;  /* column index within the row */

    /* Handle the special boundary cases first. */
    if (k == r) {                 /* last element of the row */
        return 1;
    }
    if (k == r - 1) {             /* second to last element */
        seq_t p = pow2(r + 1);    /* 2^(r+1) */
        return p - (seq_t)2;
    }

    /* For 0 <= k < r-1 use the explicit formula. */
    seq_t bin = binomial(r + 1, k + 2);       /* binomial(r+1, k+2) */
    seq_t powk = pow2(k);                    /* 2^k */
    seq_t factor = (seq_t)4 * powk - (seq_t)2; /* 4*2^k - 2 */
    return bin * factor;
}
