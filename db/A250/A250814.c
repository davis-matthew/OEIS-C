/*
 * A250814  Number of (2+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 10*n^3 + 37*n^2 + 54*n + 27
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* Compute powers of n incrementally */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    /* Apply the polynomial */
    seq_t res = n4 + (seq_t)10 * n3 + (seq_t)37 * n2 +
                (seq_t)54 * nn + (seq_t)27;

    return res;
}
