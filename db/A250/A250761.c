/*
 * A250761  Number of (6+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2607*n^2 + 4791*n + 2187 (Sequence A250761)
 * Range:     u8 none, u16 n <= 4, u32 n <= 209, u64 n <= 209, u128 n <= 209,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, this case is unused */
    seq_t nn = (seq_t)n;
    /* Compute using the quadratic formula.  All intermediate
       operations are performed in seq_t to avoid early overflow.  */
    seq_t term = (seq_t)2607 * nn * nn + (seq_t)4791 * nn + (seq_t)2187;
    return term;
}
