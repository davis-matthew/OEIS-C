/*
 * A309557  Number triangle with T(n,k) = 2 + 3*n - 2*k + 2*k*(n-k) for n >=
 *          0, 0 <= k <= n.
 * https://oeis.org/A309557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) * where r and c are the row and column of the n‑th
 *            element in the * triangle T(r,c) = 2 + 3*r - 2*c + 2*c*(r-c), *
 *            with rows indexed from 0 and columns from 0 to r. The index n *
 *            counts terms row‑by‑row starting at 0
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard headers needed for the implementation. */
#include <stdint.h>
#include <math.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
     * Use the closed form inverse of triangular numbers. */
    long double t = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((t - 1.0L) / 2.0L);

    /* Number of elements before row r. */
    idx_t prev = r * (r + 1) / 2;

    /* Column index c within the current row. */
    idx_t c = n - prev;

    /* Compute the triangle entry using seq_t arithmetic. */
    seq_t sr = (seq_t)r;
    seq_t sc = (seq_t)c;
    return (seq_t)2 + (seq_t)3 * sr - (seq_t)2 * sc + (seq_t)2 * sc * (sr - sc);
}
