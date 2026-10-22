/*
 * A320086  Triangle read by rows, 0 <= k <= n: T(n,k) is the denominator of
 *          the derivative of the k-th Bernstein basis polynomial of degree n
 *          evaluated at the interval midpoint t = 1/2; numerator is A320085.
 * https://oeis.org/A320086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   pow2, v2_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute term of OEIS A320086
 * a(n) = T(r,k) where n = r*(r+1)/2 + k, 0 <= k <= r
 * T(r,k) = 2^(r-1) / gcd(r * |C(r-1,k-1) - C(r-1,k)| , 2^(r-1))
 * The value is a power of two, computed without big integers.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* pow2(exp) returns seq_t */
#include "../utils/v2_binomial.h"

/* integer square‑root of a 64‑bit value */
static idx_t uint64_isqrt(idx_t v) {
    idx_t lo = 0, hi = (idx_t)1ULL << 32;  /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((mid * mid) <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* v2(x) = exponent of 2 in x (x > 0) */
static idx_t v2_uint64(idx_t x) {
    idx_t c = 0;
    while ((x & 1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* compute T(r,k) as defined above */
static seq_t T_row_col(idx_t r, idx_t k) {
    if (r == 0)                     /* row 0 */
        return (seq_t)1;

    if (k == 0 || k == r) {         /* boundary values */
        idx_t exp = (r - 1) - v2_uint64(r);
        return pow2(exp);
    }

    if ((2 * k) == r)               /* middle of even row, diff = 0 */
        return (seq_t)1;

    /* compute exponent of 2 in the numerator n*Δ */
    idx_t v2r   = v2_uint64(r);
    idx_t v2k   = v2_uint64(k);
    idx_t diff  = (2 * k > r) ? (2 * k - r) : (r - 2 * k);
    idx_t v2diff = v2_uint64(diff);
    idx_t v2c    = v2_binomial(r - 1, k - 1);

    idx_t v2num = v2r + v2diff + v2c - v2k;          /* exponent in numerator */
    idx_t exp   = (r > 1 && v2num < (r - 1)) ? (r - 1 - v2num) : 0;
    return pow2(exp);
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0)      /* offset 0 */
        return (seq_t)1;

    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t d = 8 * n + 1;
    idx_t sqrt_d = uint64_isqrt(d);
    idx_t r = (sqrt_d - 1) / 2;

    /* compute column k */
    idx_t start = r * (r + 1) / 2;   /* elements before row r */
    idx_t k = n - start;

    return T_row_col(r, k);
}
