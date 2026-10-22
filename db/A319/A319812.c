/*
 * A319812  Square array read by antidiagonals: T(n,k) = (1 + i)-adic
 *          valuation of n + k*i, n >= 0, k >= 0, or -1 if n + k*i = 0.
 * https://oeis.org/A319812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(0) = -1; For n > 0, a(n) = 2‑adic valuation of i² + j², where
 *            (i,j) are the coordinates obtained by reading the square array
 *            of v₂(x² + y²) along antidiagonals (i + j = d, 0 ≤ i ≤ d)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned 128‑bit integer. */
static idx_t isqrt_uint128(unsigned __int128 x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)-1;          /* maximum 64‑bit value */

    while (low <= high) {
        idx_t  mid = low + ((high - low) >> 1);
        unsigned __int128 msq = (unsigned __int128)mid * mid;
        if (msq == x) return mid;
        if (msq < x)  low  = mid + 1;
        else          high = mid - 1;
    }
    return high;
}

/* 2‑adic valuation of a non‑negative seq_t. */
static seq_t v2(seq_t x)
{
    seq_t v = 0;
    /* x is non‑negative */
    while ((x & 1) == 0) {
        x >>= 1;
        ++v;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;    /* the zero Gaussian integer */

    /* Find the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2.
       d = floor((-1 + sqrt(1+8n))/2) */
    unsigned __int128 m = (unsigned __int128)8 * n + 1;
    idx_t     d  = (isqrt_uint128(m) - 1) >> 1;

    /* Starting index of this diagonal */
    unsigned __int128 prod = (unsigned __int128)d * (d + 1);
    idx_t start = prod / 2;

    /* Offset within diagonal */
    idx_t offset = n - start;
    idx_t i = offset;
    idx_t j = d - offset;

    /* Compute T(i,j) = v₂(i² + j²) for (i,j) ≠ (0,0) */
    seq_t sq_i = (seq_t)i * (seq_t)i;
    seq_t sq_j = (seq_t)j * (seq_t)j;
    seq_t sum  = sq_i + sq_j;

    return v2(sum);
}
