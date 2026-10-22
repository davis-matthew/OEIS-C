/*
 * A358293  Array read by antidiagonals: T(n,k) = n^3*k^3*(n+k)^2, n>=1, k>=1.
 * https://oeis.org/A358293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i^3 · j^3 · (i + j)^2 where (i, j) is obtained from * the
 *            antidiagonal read of the array T(i, j) = i^3 · j^3 · (i + j)^2.
 *            * The indices i, j start at 1. The order follows the standard *
 *            antidiagonal enumeration: for each sum s = i + j, the pairs *
 *            (1, s-1), (2, s-2), …, (s-1, 1) are listed...
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static idx_t floor_isqrt(idx_t n)
{
    /* Compute floor(sqrt(n)) using long double approximation and
     * correcting with integer arithmetic.  This is safe for all
     * n that fit in idx_t. */
    idx_t r = (idx_t)sqrt((long double)n);
    while ((unsigned __int128)(r + 1) * (r + 1) <= n) ++r;
    while ((unsigned __int128)r * r > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the diagonal number d (summing i+j = d + 1) that contains
     * the n-th term.  The cumulative count up to diagonal d is
     * (d-1)d/2.  We solve for the smallest d with (d-1)d/2 >= n. */
    idx_t d = (1 + floor_isqrt(1 + 8 * n)) / 2;
    while ((unsigned __int128)(d - 1) * d / 2 < n) ++d;

    /* Compute the index of the entry within diagonal d. */
    idx_t prev = (d - 2) * (d - 1) / 2;               /* terms before d */
    idx_t pos  = n - prev - 1;                        /* 0‑based position in this diagonal */
    idx_t ni   = pos + 1;                              /* i = 1 … d-1 */
    idx_t ki   = d - ni;                               /* j = d - i */

    seq_t i = (seq_t)ni;
    seq_t j = (seq_t)ki;

    seq_t i3 = i * i * i;
    seq_t j3 = j * j * j;
    seq_t sum = i + j;
    seq_t sum2 = sum * sum;

    seq_t res = i3 * j3;
    res *= sum2;
    return res;
}
