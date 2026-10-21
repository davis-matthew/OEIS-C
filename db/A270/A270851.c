/*
 * A270851  Number of 2X2X2 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1, n or n-1 exactly once.
 * https://oeis.org/A270851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=9*n^2-42*n+48 if n even, 9*n^2-42*n+51 if
 *            n odd
 * Range:     u8 n <= 7, u16 n <= 87, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;       /* n^2 */
    val = val * 9;             /* 9*n^2 */
    val -= 42 * nn;            /* 9*n^2 - 42*n */

    if ((n & 1) == 0)          /* even n */
        val += 48;
    else                       /* odd n */
        val += 51;

    return val;
}
