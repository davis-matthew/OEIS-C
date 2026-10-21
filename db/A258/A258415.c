/*
 * A258415  Array A read by upward antidiagonals in which the entry in row n
 *          and column k is defined by A(n,k) = (2 + 2^(n-1)*(6*k - 3 +
 *          2*(-1)^n))/3, n,k >= 1.
 * https://oeis.org/A258415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(n) is obtained by reading the array A(n,k) by upward
 * antidiagonals.  A(n,k) = (2 + 2^(n-1)*(6*k - 3 + 2*(-1)^n))/3,
 * n,k >= 1.  For a given 1‑based index m (the requested term a(m)),
 * locate the antidiagonal (sum s = n+k) that contains the element,
 * then determine the corresponding (n,k) and evaluate A(n,k). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t m)
{
    if (m == 0) return 0;      /* safety; sequence is defined for m>=1 */

    /* --- locate the antidiagonal ------------------------------------ */
    /* s is the minimal integer with (s-1)*s/2 >= m */
    idx_t s = (idx_t)((1 + sqrtl((long double)(1) + 8.0L * (long double)m)) / 2);
    if (s < 2) s = 2;
    /* adjust for any rounding inaccuracies (very few iterations) */
    while (((unsigned __int128)(s - 1) * s / 2) < (unsigned __int128)m) ++s;

    /* total elements before this antidiagonal */
    unsigned __int128 prev = (unsigned __int128)(s - 2) * (s - 1) / 2;
    /* offset within the antidiagonal (0 based) */
    idx_t offset = (idx_t)(m - prev - 1);

    /* coordinates in the array:   n decreases from s-1 to 1 */
    idx_t n = (s - 1) - offset;          /* row index (>=1) */
    idx_t k = s - n;                     /* column index (>=1) */

    /* --- evaluate A(n,k) ------------------------------------------- */
    seq_t pow_val = pow2(n - 1);         /* 2^(n-1) */
    /* 6*k - 3 + 2*(-1)^n */
    seq_t t = (seq_t)6 * (seq_t)k - (seq_t)3;
    if ((n & 1ULL) == 0)                 /* even n: (-1)^n = 1 */
        t += (seq_t)2;
    else                                 /* odd n:  (-1)^n = -1 */
        t -= (seq_t)2;

    seq_t numerator = (seq_t)2 + pow_val * t;
    seq_t result = numerator / (seq_t)3; /* exact division */
    return result;
}
