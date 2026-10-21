/*
 * A295130  a(n) = 3*n*(64*n^2 + 1).
 * https://oeis.org/A295130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(64*n^2 + 1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 281, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based. Convert to seq_t before any arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute 64*n^2 + 1 */
    seq_t t = (seq_t)64 * nn * nn + (seq_t)1;

    /* Compute 3*n * t */
    seq_t result = (seq_t)3 * nn * t;

    return result;
}
