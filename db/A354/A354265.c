/*
 * A354265  Array read by ascending antidiagonals for n >= 0 and k >= 0.
 *          Generalized Lucas numbers, L(n, k) = (psi^(k - 1)*(phi + n) -
 *          phi^(k - 1)*(psi + n)), where phi = (1 + sqrt(5))/2 and psi = (1 -
 *          sqrt(5))/2.
 * https://oeis.org/A354265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = L(row, col) where L(n,k) = n*Lucas(k+1)+Lucas(k) and
 *            (row, col) is the position of n when the matrix L(n,k) is read
 *            by ascending antidiagonals
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static table of Lucas numbers up to a small fixed size.  The
   antidiagonal read of the sequence needs only Lucas numbers for
   indices up to the diagonal of the largest requested n.  For n
   up to about 10 000 the diagonal is < 150, so a table of a few
   thousand entries is more than sufficient. */
static seq_t lucas[2000];
static idx_t lucas_len = 0;  /* number of entries that are already
                                initialised in lucas[] */

/* Initialise Lucas numbers up to index 'upto' (inclusive).  The
   smallest needed indices are 0 and 1. */
static void ensure_lucas(idx_t upto)
{
    if (lucas_len > upto) return;          /* already enough */

    if (lucas_len == 0) {
        lucas[0] = (seq_t)2;               /* L(0,0) */
        lucas[1] = (seq_t)1;               /* L(0,1) */
        lucas_len = 2;
    }

    for (idx_t i = lucas_len; i <= upto; ++i) {
        lucas[i] = lucas[i-1] + lucas[i-2];
    }
    lucas_len = upto + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that the (d+1)-st triangular
       number is the first > n.   (Triangular numbers are
       T_d = (d+1)(d+2)/2.)                       */
    idx_t d = 0;
    idx_t t = 1;           /* T_0 = 1 */
    while (t <= n) {
        ++d;
        t += d + 1;         /* increment by the next term in the triangular
                               sequence: T_{d} = T_{d-1} + d+1          */
    }

    /* Number of elements before this diagonal */
    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev;          /* 0 … d */

    idx_t row = d - offset;           /* n'  */
    idx_t col = offset;               /* k'  */

    /* Compute Lucas numbers up to col+1 */
    ensure_lucas(col + 1);

    /* L(row, col) = row * Lucas(col+1) + Lucas(col) */
    seq_t result = (seq_t)row * lucas[col + 1] + lucas[col];
    return result;
}
