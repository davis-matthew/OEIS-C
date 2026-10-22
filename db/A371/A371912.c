/*
 * A371912  Maximum Zagreb index of maximal 3-degenerate graphs with n
 *          vertices.
 * https://oeis.org/A371912
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1)^2 + 9*(n-3) (for n >= 3)
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Below the offset the behaviour is irrelevant; return 0. */
    if (n < 3)
        return 0;

    seq_t t = (seq_t)n;
    seq_t n1 = t - 1;          /* n - 1        */
    seq_t n3 = t - 3;          /* n - 3        */

    /* 3*(n-1)^2 + 9*(n-3) */
    seq_t val = 3 * (n1 * n1) + 9 * n3;
    return val;
}
