/*
 * A294519  Convolution triangle for Chebyshev S polynomials (rising powers).
 * https://oeis.org/A294519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the n‑th element of the flattened convolution * triangle
 *            T(r,m) of Chebyshev S polynomials. * For a row index r and
 *            column m (0 ≤ m ≤ r): * if (r‑m) is odd then T(r,m)=0, * else
 *            let t=(r‑m)/2 * T(r,m)=(-1)^t * (t+1) * binomial(r‑t+1, t+1)
 * Range:     i8 n <= 47, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/isqrt.h"
#include <stdint.h>
#include <stdlib.h>

/* Binary search for the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
static idx_t find_row(idx_t n) {
    idx_t lo = 0, hi = n;        /* r never exceeds n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Determine the row r and the offset m within that row. */
    idx_t r = find_row(n);
    __uint128_t row_start = (__uint128_t)r * (r + 1) / 2;
    idx_t m = n - (idx_t)row_start;

    /* Compute T(r,m).  */
    idx_t diff = r - m;
    if ((diff & 1) != 0)          /* (r-m) odd → 0 */
        return 0;

    idx_t t   = diff >> 1;        /* t = (r-m)/2 */
    seq_t val = binomial(r - t + 1, t + 1);
    val *= (seq_t)(t + 1);        /* multiply by (t+1) */
    if (t & 1)                    /* apply sign (-1)^t */
        val = -val;

    return val;
}
