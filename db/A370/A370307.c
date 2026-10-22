/*
 * A370307  a(n) = A002623(n) + n.
 * https://oeis.org/A370307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 15*n^2 + 58*n + 45/2 + (3/2)*(-1)^n)/24
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type before performing
       arithmetic to avoid overflow of idx_t operands. */
    seq_t s   = (seq_t)n;        /* n  */
    seq_t n2  = s * s;           /* n^2 */
    seq_t n3  = n2 * s;          /* n^3 */

    /* Compute the numerator: 4*n^3 + 30*n^2 + 116*n + 45 + 3*(-1)^n */
    seq_t num = (seq_t)4 * n3 + (seq_t)30 * n2 + (seq_t)116 * s + (seq_t)45;

    /* Add 3 if n is even, subtract 3 if n is odd (since 3*(-1)^n
       equals  3 for even n, -3 for odd n). */
    if ((n & 1U) == 0)  /* even n */
        num += (seq_t)3;
    else                /* odd n */
        num -= (seq_t)3;

    /* The result is divisible by 48. */
    return num / (seq_t)48;
}
