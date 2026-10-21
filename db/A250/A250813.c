/*
 * A250813  Number of (1+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 10 n^3 + 37 n^2 + 60 n + 36) / 4
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t n2 = x * x;       /* n^2 */
    seq_t n3 = n2 * x;      /* n^3 */
    seq_t n4 = n2 * n2;     /* n^4 */
    seq_t num = n4 + 10 * n3 + 37 * n2 + 60 * x + 36; /* numerator */
    return num / 4;                                   /* divide by 4 */
}
