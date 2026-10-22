/*
 * A352116  Partial sums of the odd triangular numbers (A014493).
 * https://oeis.org/A352116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2 - 1 - 3*(-1)^n)/6
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* defined only for n>=1 */

    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;          /* n^2  */
    seq_t term;                    /* 4*n^2 - 4  if n even, 4*n^2 + 2  if n odd */

    if ((n & 1U) == 0U)            /* n even */
        term = (seq_t)4 * n_sq - (seq_t)4;
    else                           /* n odd  */
        term = (seq_t)4 * n_sq + (seq_t)2;

    seq_t top = nn * term;         /* n * term  */
    return top / (seq_t)6;         /* division is exact */
}
