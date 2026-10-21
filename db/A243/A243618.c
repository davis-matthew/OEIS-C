/*
 * A243618  Table read by antidiagonals: T(n,k) is the curvature of a circle
 *          in a nested Pappus chain (see Comments for details).
 * https://oeis.org/A243618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(i,j) = 2 + i*i + j*(j+3) (i,j >= 0) The sequence a(n)
 *            enumerates the table T(i,j) along antidiagonals: for each
 *            diagonal d = i+j, the terms are (i=0,j=d), (1,d-1), …, (d,0).
 *            a(0) = T(0,0) = 2
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > n / mid) high = mid - 1;
        else                low  = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = (isqrt_u64(8 * n + 1) - 1) / 2;

    /* Position inside diagonal. */
    idx_t offset = n - (d * (d + 1)) / 2;
    idx_t i = offset;
    idx_t j = d - offset;

    /* Compute T(i,j) in seq_t. */
    seq_t val = 2;
    val += (seq_t)i * i;
    val += (seq_t)j * (j + 3);
    return val;
}
