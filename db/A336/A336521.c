/*
 * A336521  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) is the coefficient of x^(k*n) in expansion of ( (1 + x)/(1
 *          - x) )^n.
 * https://oeis.org/A336521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 98, u64 n <= 223,
 *            u128 n <= 585, big any n
 * Helpers:   binom_fn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336521.
 * a(n) is the sequence obtained by reading the square array
 * `T(r,k)` by antidiagonals.  `T(r,k)` is the coefficient of
 * x^(k*r) in ((1+x)/(1-x))^r.
 *
 * Mapping from flattened index *n* to (r,k):
 *   Find the diagonal d such that
 *       d(d+1)/2 <= n < (d+1)(d+2)/2          (1)
 *   Let   before = d(d+1)/2
 *         offset = n - before                (0‑based index in the diagonal)
 *   Then
 *         r = offset
 *         k = d - offset
 *
 *    For r==0 or k==0 the coefficient is 1.
 *
 *    For r>0 , k>0 we use the closed form
 *         T(r,k) = Σ_{j=1..r} 2^j * C(r,j) * C(k*r-1, j-1).
 *
 *    All arithmetic is done in unsigned type `seq_t`.  No
 *    floating point or overflow conditions are assumed
 *    beyond the natural wrap‑around of unsigned arithmetic.
 */
#include <stddef.h>  /* for size_t */
#include "../utils/binom_fn.h"

seq_t a(idx_t n)
{
    /* locate diagonal d: smallest d with (d+1)(d+2)/2 > n   */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t before = d * (d + 1) / 2;
    idx_t offset = n - before;      /* 0‑based position inside diagonal d */
    idx_t r = offset;               /* row index (0 .. d) */
    idx_t k = d - offset;           /* column index */

    if (r == 0 || k == 0) return (seq_t)1;

    seq_t sum  = (seq_t)0;
    seq_t pow2 = (seq_t)2;   /* 2^1 for j=1 */

    for (idx_t j = 1; j <= r; ++j) {
        seq_t bn = binom_fn(r, j);            /* C(r,j)   */
        seq_t ak = binom_fn(k * r - 1, j - 1);/* C(k*r-1, j-1) */
        sum += pow2 * bn * ak;
        pow2 <<= 1;   /* prepare 2^(j+1) for next loop */
    }
    return sum;
}
