/*
 * A250756  Number of (1+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (17*n^2 + 29*n)/2 + 9 (Equivalently: 3*a(n-1) - 3*a(n-2)
 *            + a(n-3) for n > 3, with a(1)=32, a(2)=72, a(3)=129)
 * Range:     u8 n <= 3, u16 n <= 62, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is not part of the sequence; return 0 for consistency. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)17 * nn * nn + (seq_t)29 * nn;
    val /= 2;
    val += 9;
    return val;
}
