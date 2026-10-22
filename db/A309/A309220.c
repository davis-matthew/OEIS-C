/*
 * A309220  Square array A read by antidiagonals: the columns are given by
 *          A(n,1)=1, A(n,2)=n+1, A(n,3) = n^2+2n+3, A(n,4) = n^3+3*n^2+6*n+4,
 *          A(n,5) = n^4+4*n^3+10*n^2+12*n+7, ..., whose coefficients are
 *          given by A104509 (see also A118981).
 * https://oeis.org/A309220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array A(i,j) defined by
 *            A(i,1) = 1 A(i,2) = i + 1 A(i,3) = i^2 + 2i + 3 for j ≥ 4 :
 *            A(i,j) = (i + 1) * A(i, j-1) + A(i, j-2) The sequence is read by
 *            antidiagonals: a(1) = A(1,1) a(2) = A(2,1), a(3) = A(1,2) a(4) =
 *            A(3,1), a(5) = A(2,2), a(6) = A(1,3) ... etc
 * Range:     u8 n <= 25, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uintptr_t, may be used via idx_t/seq_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 */

    /* --- find the antidiagonal (m) that contains the nth term --- */
    idx_t m = 1;
    while ((unsigned __int128)m * (m + 1) / 2 < n) ++m;

    /* triangular number of previous diagonal: t(m-1) = (m-1)*m/2 */
    idx_t prev_tri = (idx_t)(((unsigned __int128)(m - 1) * m) / 2);
    idx_t offset   = n - prev_tri;                 /* position in diagonal (1‑based) */

    idx_t i = m - offset + 1;   /* row index (1‑based)   */
    idx_t j = offset;           /* column index (1‑based) */

    /* --- evaluate A(i,j) via the recurrence --- */
    if (j == 1) return (seq_t)1;

    if (j == 2) return (seq_t)(i + 1);

    /* j == 3 */
    if (j == 3) {
        seq_t ii = (seq_t)i;
        return ii * ii + (seq_t)2 * ii + (seq_t)3;
    }

    /* For j >= 4: iterate using the recurrence
       A(i,j) = (i+1) * A(i,j-1) + A(i,j-2)    */
    seq_t prev2 = (seq_t)(i + 1);                               /* A(i,2) */
    seq_t prev1 = (seq_t)(i * i) + (seq_t)2 * (seq_t)i + (seq_t)3; /* A(i,3) */
    seq_t cur;

    for (idx_t k = 4; k <= j; ++k) {
        cur = (seq_t)(i + 1) * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
