/*
 * A364823  Triangle read by rows: T(n,k) = number of possible positions for
 *          four connected discs in the game "Connect Four" played on a board
 *          with n columns and k rows, 4 <= k <= n.
 * https://oeis.org/A364823
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 4*k*n - 9*k - 9*n + 18, where n and k are determined from
 *            the flattened triangle of T(n,k) (k ranges 4..n, rows read by
 *            increasing n)
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 4.  Return 0 for earlier indices. */
    if (n < 4)
        return (seq_t)0;

    /* Zero‑based offset in the flattened list, starting with a(4). */
    idx_t offset = n - 4;

    /* Find the row number N such that the cumulative count of terms
       up to row N exceeds offset.  Cum(N) = (N-3)(N-2)/2.  We
       solve t(t+1)/2 > offset where t = N-3.  The largest t with
       t(t+1)/2 <= offset is given by t = ⌊(√(1+8·offset) – 1)/2⌋.
       Then the desired row is t+4. */
    idx_t s = offset * 8 + 1;                /* 1 + 8·offset fits in uint64_t for the limits used. */
    long double d = sqrt((long double)s);
    idx_t sqrt_s = (idx_t)d;                /* floor of the square root. */
    idx_t t_base  = (sqrt_s - 1) / 2;        /* largest t with t(t+1)/2 <= offset. */
    idx_t N       = t_base + 4;              /* target row n. */

    /* Offset of the first element of this row. */
    idx_t row_start = (t_base * (t_base + 1)) / 2;   /* cumulative count up to previous row. */

    /* Position within the row (0‑based).  k runs from 4 up to N. */
    idx_t p = offset - row_start;            /* 0 <= p <= N-4. */
    idx_t k = 4 + p;                          /* column index. */

    /* Compute the triangle entry. */
    seq_t kt = (seq_t)k;
    seq_t nt = (seq_t)N;
    return 4 * kt * nt - 9 * kt - 9 * nt + 18;
}
