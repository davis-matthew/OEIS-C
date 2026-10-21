/*
 * A250879  Number of (2+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 55*n^3 + 155*n^2 + 166*n + 64
 * Range:     u8 none, u16 n <= 9, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 the function
       simply returns 0 – this value is never used by the test harness. */
    if (n == 0) return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    return (seq_t)55 * n3 + (seq_t)155 * n2 + (seq_t)166 * nn + (seq_t)64;
}
