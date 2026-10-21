/*
 * A257791  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) = 2^(n+1)*(2*k -
 *          1), n,k >= 1.
 * https://oeis.org/A257791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element in the upward antidiagonal traversal of
 *            the rectangular array A(n,k)=2^(n+1)*(2*k-1)
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* offset is 1; 0 is unused */

    /* Find the diagonal d such that d*(d+1)/2 >= n  */
    idx_t low  = 1;
    idx_t high = n;            /* high is certainly enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                 /* diagonal index */

    /* Number of elements before this diagonal */
    __int128 prev128 = (__int128)(d - 1) * d / 2;
    idx_t prev = (idx_t)prev128;

    /* Position inside the diagonal (1‑based) */
    idx_t p   = n - prev;          /* 1 <= p <= d */

    /* Convert to matrix coordinates: (row, col) */
    idx_t row = d - p + 1;         /* row index >=1 */
    idx_t col = p;                 /* column index >=1 */

    /* Compute the value: 2^(row+1) * (2*col - 1) */
    seq_t mult = (seq_t)(2 * col - 1);      /* 2*col-1 fits in seq_t */
    seq_t value = pow2(row + 1) * mult;

    return value;
}
