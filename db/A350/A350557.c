/*
 * A350557  Triangle T(n,k) read by rows with T(n,0) = (2*n)! / (2^n * n!) for
 *          n >= 0 and T(n,k) = (Sum_{i=k..n} binomial(i-1,k-1) * 2^i * i! /
 *          (2*i)!) * (2*n)! / (2^n * n!) for 0 < k <= n.
 * https://oeis.org/A350557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Triangle T(n,k) read by rows; a(n) is the n-th element of this
 *            triangle. T(n,0) = (2*n)! / (2^n * n!) T(n,k) =
 *            binomial(n-1,k-1) + (2*n - 1) * T(n-1,k) for 0 < k < n T(n,n) =
 *            1 a(n) = T(r,k) where r = smallest integer with (r+1)(r+2)/2 >
 *            n, k = n - r(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative size up to r-1 <= n < cumulative up to r. */
    idx_t r = 0;
    while (( (__int128)(r + 1) * (r + 2) ) / 2 <= ( __int128 )n)
        ++r;

    /* Number of terms before row r */
    idx_t prev = ( (__int128)r * (r + 1) ) / 2;
    idx_t k = n - prev;   /* column index within the row */

    /* Terminal cases */
    if (k == r)                 /* T(r,r) = 1 */
        return (seq_t)1;

    if (k == 0) {               /* T(r,0) = (2*r)! / (2^r * r!) */
        seq_t num   = factorial((idx_t)(2 * r));
        seq_t denom = pow2(r) * factorial(r);
        return num / denom;
    }

    /* General case: compute T(r,k) by recurrence, starting from T(k,k) = 1. */
    seq_t t = (seq_t)1;         /* current T(i-1,k), starting at i = k+1 */
    for (idx_t i = k + 1; i <= r; ++i) {
        seq_t bin  = binomial(i - 1, k - 1);
        seq_t mul  = (seq_t)(2 * i - 1);
        t = bin + mul * t;
    }
    return t;
}
