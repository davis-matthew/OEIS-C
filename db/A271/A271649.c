/*
 * A271649  a(n) = 2*(n^2 - n + 2).
 * https://oeis.org/A271649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 - n + 2)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 in this case. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t val = nn * nn - nn + 2;
    return (seq_t)2 * val;
}
