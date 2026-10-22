/*
 * A398438  Array read by ascending antidiagonals: A(0,k) = 0 and A(n,k) =
 *          A(n-1,k)^2 + k for n > 0.
 * https://oeis.org/A398438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(0,k) = 0 * A(n,k) = A(n-1,k)^2 + k for n > 0 * The sequence
 *            a(n) reads the two‑dimensional array A on * ascending
 *            antidiagonals, but each diagonal is traversed * from the largest
 *            i to 0 (row decreasing), i.e. * (i,j) = (s-d, d) for d = n -
 *            s(s+1)/2, s = floor((sqrt(8n+1)-1)/2). * * Example: a(0...
 * Range:     u8 n <= 21, u16 n <= 28, u32 n <= 36, u64 n <= 45, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* locate the antidiagonal (s) and position (d) on it */
    double t = sqrt((double)n * 8.0 + 1.0);
    idx_t s = (idx_t)((t - 1.0) / 2.0);          /* diagonal index */
    idx_t start = s * (s + 1) / 2;              /* first index on this diagonal */
    idx_t d = n - start;                       /* offset within diagonal */

    /* coordinates: i = s - d (row), j = d (column) */
    idx_t i = s - d;
    idx_t j = d;

    /* compute A(i,j) by iterating the recurrence */
    seq_t val = 0;
    for (idx_t r = 1; r <= i; ++r)
        val = val * val + (seq_t)j;            /* val = val^2 + j */

    return val;
}
