/*
 * A382973  a(n) = 4*n^3 - 6*n^2 + 6*n - 2 + (-1)^n.
 * https://oeis.org/A382973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 6*n^2 + 6*n - 2 + (-1)^n
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */

    /* Base polynomial part */
    seq_t value = (seq_t)4 * n3
                - (seq_t)6 * n2
                + (seq_t)6 * nn
                - (seq_t)2;

    /* Add (-1)^n: +1 for even n, -1 for odd n */
    if (n & 1)
        value -= (seq_t)1;       /* odd n */
    else
        value += (seq_t)1;       /* even n */

    return value;
}
