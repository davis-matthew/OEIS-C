/*
 * A320085  Triangle read by rows, 0 <= k <= n: T(n,k) is the numerator of the
 *          derivative of the k-th Bernstein basis polynomial of degree n
 *          evaluated at the interval midpoint t = 1/2; denominator is
 *          A320086.
 * https://oeis.org/A320085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl, frac
 * Formula:   not stated
 * Range:     i8 n <= 46, i16 n <= 164, i32 n <= 644, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A320085
 *
 *   a(n) = the n-th entry of the triangle T(n,k) read by rows.
 *
 *   T(n,k) = numerator of n*(C(n-1,k-1)-C(n-1,k)) / 2^(n-1), reduced
 *   by cancelling all powers of 2 (i.e. T(n,k) = odd part of n*(C(n-1,k-1)-C(n-1,k))).
 *
 *   The sequence begins
 *     0, -1, 1, -1, 0, 1, -3, -3, 3, 3, -1, -1, 0, 1, 1, …
 *
 *   The function below computes a(n) for any n ≥ 0, using only integer
 *   arithmetic and the supplied binomial routine.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square‑root of a 64‑bit unsigned integer (floor).  Uses 128‑bit
 * arithmetic to avoid overflow.  The result is guaranteed to be ≤ 2^32. */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Find the row number r such that   r*(r+1)/2 ≤ n < (r+1)*(r+2)/2    */
static inline idx_t row_of(idx_t n)
{
    if (n == 0) return 0;
    idx_t d = 1 + 8 * n;
    idx_t s = floor_isqrt_u64(d);
    idx_t r = (s - 1) / 2;

    /* Adjust in case of rounding errors */
    while (((__uint128_t)r * (r + 1)) / 2 > n) --r;
    while (((__uint128_t)(r + 1) * (r + 2)) / 2 <= n) ++r;
    return r;
}

/* Compute the triangular entry T(n,k).  n>0 is required.  The value is
 * the odd part of n*(C(n-1,k-1)-C(n-1,k)).  For k==0 or k==n the
 * difference reduces to ±1. */
static seq_t triangular_entry(idx_t n, idx_t k)
{
    if (n == 0) return 0;          /* only entry is 0 */

    seq_t diff;
    if (k == 0) {
        diff = -(seq_t)1;
    } else if (k == n) {
        diff = (seq_t)1;
    } else {
        seq_t b1 = binomial(n - 1, k - 1);
        seq_t b2 = binomial(n - 1, k);
        diff = b1 - b2;
    }

    seq_t val = (seq_t)n * diff;    /* may be negative */

    /* Strip all factors of 2 (i.e. obtain the odd part) */
    if (val == 0) return 0;
    while ((val & 1) == 0) {
        val >>= 1;
    }
    return val;
}

/* Public function required by the build.  It returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Determine which row and column the flattened index n refers to. */
    idx_t r   = row_of(n);
    idx_t prev = (idx_t)(((__uint128_t)r * (r + 1)) / 2);  /* entries before row r */
    idx_t k   = n - prev;  /* column within the row (0‑based) */

    return triangular_entry(r, k);
}
