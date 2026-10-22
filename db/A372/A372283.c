/*
 * A372283  Array read by upward antidiagonals: A(n, k) = R(A(n-1, k)) for n >
 *          1, k >= 1; A(1, k) = 2*k-1, where Reduced Collatz function R(n)
 *          gives the odd part of 3n+1.
 * https://oeis.org/A372283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read by upward antidiagonals of the array A(1,k)=2k-1,
 *            A(n,k)=odd_part(3*A(n-1,k)+1); i.e. for each n find the diagonal
 *            (row+col-1)=d with d(d+1)/2≥n, set col=p=n−d(d−1)/2 and
 *            row=d−p+1, then start from v=2·col−1 and apply R=v →
 *            odd_part(3v+1) (row−1) times
 * Range:     u8 n <= 391, u16 n <= 9727, u32 n <= 9727, u64 n <= 9727,
 *            u128 n <= 9727, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_part.h"

/* Compute a(n) for n≥1.  The function is safe for any n that fits
   in idx_t; for very large n the loop might get slow but the
   requirements only demand n≈10 000. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard, offset is 1 */
    /* Find the diagonal d such that d(d+1)/2 ≥ n.  For n up to 10 000
       this loop is negligible (d ≤ 142). */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    /* Number of terms before the current diagonal */
    idx_t t_prev = d * (d - 1) / 2;

    /* Position inside the diagonal (1 … d) */
    idx_t p = n - t_prev;          /* 1‑based index in the diagonal */

    /* Column is the position, row decreases up the diagonal */
    idx_t col = p;                 /* 1 … d */
    idx_t row = d - p + 1;         /* 1 … d */

    /* Initial value in column col */
    seq_t v = (seq_t)2 * (seq_t)col - (seq_t)1;

    /* Apply the Collatz reduction row‑1 times */
    for (idx_t r = 2; r <= row; ++r)
        v = odd_part( ((seq_t)3 * v) + (seq_t)1 );

    return v;
}
