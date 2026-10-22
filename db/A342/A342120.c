/*
 * A342120  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/(1 - k*x - k*x^2).
 * https://oeis.org/A342120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 83, u32 n <= 181, u64 n <= 452,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A342120 – square array by antidiagonals
 *
 *  T(0,k) = 1
 *  T(1,k) = k
 *  T(n,k) = k * (T(n-1,k) + T(n-2,k))  ,  n > 1
 *
 *  a(n) is obtained by reading this array along antidiagonals:
 *  a(n) = T(i, j)  where
 *      d   = largest integer with d*(d+1)/2 <= n
 *      i   = n - d*(d+1)/2          (position inside the d‑th diagonal)
 *      j   = d - i
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t low  = 0;
    idx_t high = n;             /* high is inclusive */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;          /* upper mid */
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t d = low;

    /* Position inside the diagonal */
    idx_t prev = d * (d + 1) / 2;  /* triangular number of previous diagonals */
    idx_t pos  = n - prev;          /* 0 <= pos <= d */

    idx_t i = pos;        /* row index */
    idx_t j = d - pos;    /* column index */

    /* Compute T(i,j) by the recurrence */
    if (i == 0) return (seq_t)1;
    if (i == 1) return (seq_t)j;

    seq_t t0 = (seq_t)1;          /* T(0,j) */
    seq_t t1 = (seq_t)j;          /* T(1,j) */
    for (idx_t r = 2; r <= i; ++r) {
        seq_t t2 = (seq_t)j * (t1 + t0);
        t0 = t1;
        t1 = t2;
    }
    return t1;
}
