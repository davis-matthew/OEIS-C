/*
 * A398062  Array read by ascending antidiagonals: A(n,k) is the n-th
 *          nondivisor of k.
 * https://oeis.org/A398062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = the (row)-th nondivisor of k, where (row,k) is obtained
 *            from the index n by reading the infinite array A(row,k) = row-th
 *            nondivisor of k in reverse anti‑diagonal order
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   nth_nondivisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/nth_nondivisor.h"

/* Main function as required. */
seq_t a(idx_t n)
{
    /* Find diagonal d such that T(d-1) < n <= T(d), where T(d)=d*(d+1)/2 */
    idx_t d = 1;
    idx_t tri = 1;       /* T(d) for current d */
    while (tri < n) {
        d++;
        tri += d;        /* T(d) = T(d-1) + d */
    }
    /* Offset within this diagonal, 0‑based. */
    idx_t prev_tri = tri - d;          /* T(d-1) */
    idx_t offset = n - prev_tri - 1;   /* 0 <= offset < d */

    /* Coordinates in reverse order: row = d - offset, col = offset + 1. */
    idx_t row = d - offset;
    idx_t col = offset + 1;

    return nth_nondivisor(col, row);
}
