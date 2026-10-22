/*
 * A348228  Partial sums of A348227.
 * https://oeis.org/A348228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n^2 + 2*n - 17 + (2*n - 3)*(-1)^n)/4 for n >= 3
 *            Initial values: a(0) = 1, a(1) = 5, a(2) = 17
 * Range:     u8 n <= 3, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)17;

    seq_t nn = (seq_t)n;
    /* Compute the numerator: 18*n^2 + 2*n - 17 */
    seq_t num = (seq_t)18 * nn * nn + (seq_t)2 * nn - (seq_t)17;

    /* Add or subtract (2*n - 3) depending on (-1)^n.
       (-1)^n is +1 for even n and -1 for odd n. */
    seq_t add = (seq_t)2 * nn - (seq_t)3;
    if ((n & 1) == 0) {          /* n is even: add the term */
        num += add;
    } else {                     /* n is odd: subtract the term */
        num -= add;
    }

    /* Divide by 4 to obtain the final value.  The division is exact. */
    return num / (seq_t)4;
}
